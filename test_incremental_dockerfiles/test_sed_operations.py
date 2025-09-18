#!/usr/bin/env python3
"""
Dockerfile Marker System Test Suite

Tests all sed operations for incremental build Dockerfile management.
Validates: F->I, I->F, I->I, F->F transitions plus idempotent operations.
"""

import subprocess
import shutil
import os
from pathlib import Path

class DockerfileTestSuite:
    def __init__(self):
        self.test_count = 0
        self.pass_count = 0
        self.fail_count = 0

    def run_test(self, name, description):
        self.test_count += 1
        print(f"\n🧪 Test {self.test_count}: {name}")
        print(f"Description: {description}")
        print("-" * 50)

    def assert_line(self, expected, actual, description):
        if actual.strip() == expected.strip():
            print(f"✅ PASS: {description}")
            self.pass_count += 1
        else:
            print(f"❌ FAIL: {description}")
            print(f"   Expected: '{expected.strip()}'")
            print(f"   Actual:   '{actual.strip()}'")
            self.fail_count += 1

    def get_line_after_marker(self, dockerfile, marker):
        """Get the line immediately after a marker"""
        with open(dockerfile, 'r') as f:
            lines = f.readlines()

        for i, line in enumerate(lines):
            if line.strip() == marker:
                if i + 1 < len(lines):
                    return lines[i + 1].rstrip()
        return ""

    def get_run_line(self, dockerfile):
        """Get the RUN echo line for incremental deploy"""
        with open(dockerfile, 'r') as f:
            for line in f:
                if "RUN echo" in line and "INCREMENTAL DEPLOY" in line:
                    return line.rstrip()
        return ""

    def apply_incremental_sed(self, dockerfile, binary_name):
        """Apply the exact sed commands from build.sh for INCREMENTAL mode"""
        # Binary filename update and uncomment (handles both commented and uncommented COPY lines)
        subprocess.run([
            'sed', '-i',
            f'/^# COPY_BINARY_MARKER$/{{ n; s/^# *COPY .*/COPY {binary_name} \\/tmp\\/Hyprland-incremental/; s/^COPY .*/COPY {binary_name} \\/tmp\\/Hyprland-incremental/; }}',
            dockerfile
        ], check=True)

        # Version file uncomment
        subprocess.run([
            'sed', '-i',
            '/^# COPY_VERSION_MARKER$/{ n; s/^# *COPY/COPY/; }',
            dockerfile
        ], check=True)

        # RUN block uncomment
        subprocess.run([
            'sed', '-i',
            '/^# RUN echo.*INCREMENTAL DEPLOY/,/^# INCREMENTAL_BINARY_COPY_END/s/^# //',
            dockerfile
        ], check=True)

    def apply_standard_sed(self, dockerfile):
        """Apply the exact sed commands from build.sh for STANDARD mode"""
        # Binary COPY comment and restore template
        subprocess.run([
            'sed', '-i',
            '/^# COPY_BINARY_MARKER$/{ n; s/^COPY .*/# COPY Hyprland-VERSION \\/tmp\\/Hyprland-incremental/; }',
            dockerfile
        ], check=True)

        # Version file comment
        subprocess.run([
            'sed', '-i',
            '/^# COPY_VERSION_MARKER$/{ n; s/^COPY/# COPY/; }',
            dockerfile
        ], check=True)

        # RUN block comment
        subprocess.run([
            'sed', '-i',
            '/^RUN echo.*INCREMENTAL DEPLOY/,/^INCREMENTAL_BINARY_COPY_END/s/^/# /',
            dockerfile
        ], check=True)

    def test_standard_to_incremental(self):
        """Test F->I: STANDARD → INCREMENTAL transition"""
        self.run_test("STANDARD → INCREMENTAL", "Enable incremental deployment from standard template")

        shutil.copy('test_dockerfile_standard.txt', 'test_f_to_i.txt')
        self.apply_incremental_sed('test_f_to_i.txt', 'Hyprland-0.41.2+ds-1.3+step8.9.42')

        binary_line = self.get_line_after_marker('test_f_to_i.txt', '# COPY_BINARY_MARKER')
        version_line = self.get_line_after_marker('test_f_to_i.txt', '# COPY_VERSION_MARKER')
        run_line = self.get_run_line('test_f_to_i.txt')

        self.assert_line('COPY Hyprland-0.41.2+ds-1.3+step8.9.42 /tmp/Hyprland-incremental', binary_line, 'Binary COPY uncommented with correct filename')
        self.assert_line('COPY HYPRMOON_VERSION.txt /tmp/', version_line, 'Version COPY uncommented')
        self.assert_line('RUN echo "🔥 INCREMENTAL DEPLOY: Clobbering with latest binary" \\', run_line, 'RUN block uncommented')

    def test_incremental_to_standard(self):
        """Test I->F: INCREMENTAL → STANDARD transition"""
        self.run_test("INCREMENTAL → STANDARD", "Disable incremental deployment back to standard")

        shutil.copy('test_dockerfile_incremental.txt', 'test_i_to_f.txt')
        self.apply_standard_sed('test_i_to_f.txt')

        binary_line = self.get_line_after_marker('test_i_to_f.txt', '# COPY_BINARY_MARKER')
        version_line = self.get_line_after_marker('test_i_to_f.txt', '# COPY_VERSION_MARKER')
        run_line = self.get_run_line('test_i_to_f.txt')

        self.assert_line('# COPY Hyprland-VERSION /tmp/Hyprland-incremental', binary_line, 'Binary COPY commented with template restored')
        self.assert_line('# COPY HYPRMOON_VERSION.txt /tmp/', version_line, 'Version COPY commented')
        self.assert_line('# RUN echo "🔥 INCREMENTAL DEPLOY: Clobbering with latest binary" \\', run_line, 'RUN block commented')

    def test_incremental_to_incremental(self):
        """Test I->I: INCREMENTAL → INCREMENTAL version update"""
        self.run_test("INCREMENTAL → INCREMENTAL", "Update binary version while maintaining incremental mode")

        shutil.copy('test_dockerfile_incremental.txt', 'test_i_to_i.txt')
        self.apply_incremental_sed('test_i_to_i.txt', 'Hyprland-0.41.2+ds-1.3+step8.9.50')

        binary_line = self.get_line_after_marker('test_i_to_i.txt', '# COPY_BINARY_MARKER')
        version_line = self.get_line_after_marker('test_i_to_i.txt', '# COPY_VERSION_MARKER')
        run_line = self.get_run_line('test_i_to_i.txt')

        self.assert_line('COPY Hyprland-0.41.2+ds-1.3+step8.9.50 /tmp/Hyprland-incremental', binary_line, 'Binary filename updated to new version')
        self.assert_line('COPY HYPRMOON_VERSION.txt /tmp/', version_line, 'Version COPY remains uncommented')
        self.assert_line('RUN echo "🔥 INCREMENTAL DEPLOY: Clobbering with latest binary" \\', run_line, 'RUN block remains uncommented')

    def test_standard_to_standard(self):
        """Test F->F: STANDARD → STANDARD version update"""
        self.run_test("STANDARD → STANDARD", "Maintain standard mode across version updates")

        shutil.copy('test_dockerfile_standard.txt', 'test_f_to_f.txt')
        self.apply_standard_sed('test_f_to_f.txt')

        binary_line = self.get_line_after_marker('test_f_to_f.txt', '# COPY_BINARY_MARKER')
        version_line = self.get_line_after_marker('test_f_to_f.txt', '# COPY_VERSION_MARKER')
        run_line = self.get_run_line('test_f_to_f.txt')

        self.assert_line('# COPY Hyprland-VERSION /tmp/Hyprland-incremental', binary_line, 'Binary COPY remains commented with template')
        self.assert_line('# COPY HYPRMOON_VERSION.txt /tmp/', version_line, 'Version COPY remains commented')
        self.assert_line('# RUN echo "🔥 INCREMENTAL DEPLOY: Clobbering with latest binary" \\', run_line, 'RUN block remains commented')

    def test_incremental_idempotent(self):
        """Test I->I->I: Multiple incremental operations should be safe"""
        self.run_test("INCREMENTAL Idempotent", "Multiple incremental enables should be safe and correct")

        shutil.copy('test_dockerfile_standard.txt', 'test_i_idempotent.txt')

        # Apply incremental mode multiple times with different versions
        self.apply_incremental_sed('test_i_idempotent.txt', 'Hyprland-0.41.2+ds-1.3+step8.9.60')
        self.apply_incremental_sed('test_i_idempotent.txt', 'Hyprland-0.41.2+ds-1.3+step8.9.61')
        self.apply_incremental_sed('test_i_idempotent.txt', 'Hyprland-0.41.2+ds-1.3+step8.9.62')

        binary_line = self.get_line_after_marker('test_i_idempotent.txt', '# COPY_BINARY_MARKER')
        version_line = self.get_line_after_marker('test_i_idempotent.txt', '# COPY_VERSION_MARKER')
        run_line = self.get_run_line('test_i_idempotent.txt')

        self.assert_line('COPY Hyprland-0.41.2+ds-1.3+step8.9.62 /tmp/Hyprland-incremental', binary_line, 'Idempotent: Final binary version correct')
        self.assert_line('COPY HYPRMOON_VERSION.txt /tmp/', version_line, 'Idempotent: Version COPY remains uncommented')
        self.assert_line('RUN echo "🔥 INCREMENTAL DEPLOY: Clobbering with latest binary" \\', run_line, 'Idempotent: RUN block remains uncommented')

    def test_standard_idempotent(self):
        """Test F->F->F: Multiple standard operations should be safe"""
        self.run_test("STANDARD Idempotent", "Multiple standard disables should be safe and correct")

        shutil.copy('test_dockerfile_incremental.txt', 'test_f_idempotent.txt')

        # Apply standard mode multiple times
        self.apply_standard_sed('test_f_idempotent.txt')
        self.apply_standard_sed('test_f_idempotent.txt')
        self.apply_standard_sed('test_f_idempotent.txt')

        binary_line = self.get_line_after_marker('test_f_idempotent.txt', '# COPY_BINARY_MARKER')
        version_line = self.get_line_after_marker('test_f_idempotent.txt', '# COPY_VERSION_MARKER')
        run_line = self.get_run_line('test_f_idempotent.txt')

        self.assert_line('# COPY Hyprland-VERSION /tmp/Hyprland-incremental', binary_line, 'Idempotent: Template restored correctly')
        self.assert_line('# COPY HYPRMOON_VERSION.txt /tmp/', version_line, 'Idempotent: Version COPY commented correctly')
        self.assert_line('# RUN echo "🔥 INCREMENTAL DEPLOY: Clobbering with latest binary" \\', run_line, 'Idempotent: RUN block commented correctly')

    def run_all_tests(self):
        """Run the complete test suite"""
        print("🔬 Dockerfile Marker System Test Suite")
        print("=" * 60)
        print("Testing sed operations for incremental build deployment")
        print()

        # Run all test cases
        self.test_standard_to_incremental()     # F->I
        self.test_incremental_to_standard()     # I->F
        self.test_incremental_to_incremental()  # I->I
        self.test_standard_to_standard()        # F->F
        self.test_incremental_idempotent()      # I->I->I
        self.test_standard_idempotent()         # F->F->F

        # Summary
        print()
        print("🏁 Test Results Summary")
        print("=" * 30)
        print(f"✅ Passed: {self.pass_count}")
        print(f"❌ Failed: {self.fail_count}")
        print(f"📊 Total:  {self.test_count} test cases")

        if self.fail_count == 0:
            print()
            print("🎉 ALL TESTS PASSED!")
            print("The marker system correctly handles all build mode transitions.")
            return True
        else:
            print()
            print("💥 SOME TESTS FAILED!")
            print("Check the sed logic in build.sh for issues.")
            return False

if __name__ == "__main__":
    # Change to test directory
    os.chdir(Path(__file__).parent)

    # Run test suite
    suite = DockerfileTestSuite()
    success = suite.run_all_tests()

    # Exit with appropriate code
    exit(0 if success else 1)