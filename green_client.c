#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <wayland-client.h>

static struct wl_display *display;
static struct wl_compositor *compositor;
static struct wl_surface *surface;
static struct wl_shm *shm;
static struct wl_shell *shell;
static struct wl_shell_surface *shell_surface;
static struct wl_buffer *buffer;

static void shm_format(void *data, struct wl_shm *wl_shm, uint32_t format) {
    // Accept format
}

static const struct wl_shm_listener shm_listener = { shm_format };

static void registry_handler(void *data, struct wl_registry *registry,
                           uint32_t id, const char *interface, uint32_t version) {
    printf("Interface: %s, version: %d, id: %d\n", interface, version, id);
    
    if (strcmp(interface, "wl_compositor") == 0) {
        compositor = wl_registry_bind(registry, id, &wl_compositor_interface, 1);
        printf("Bound compositor\n");
    } else if (strcmp(interface, "wl_shm") == 0) {
        shm = wl_registry_bind(registry, id, &wl_shm_interface, 1);
        wl_shm_add_listener(shm, &shm_listener, NULL);
        printf("Bound shm\n");
    } else if (strcmp(interface, "wl_shell") == 0) {
        shell = wl_registry_bind(registry, id, &wl_shell_interface, 1);
        printf("Bound shell\n");
    }
}

static void registry_remover(void *data, struct wl_registry *registry, uint32_t id) {}

static const struct wl_registry_listener registry_listener = {
    registry_handler, registry_remover
};

static int create_shm_file(size_t size) {
    char name[] = "/tmp/wl_shm-XXXXXX";
    int fd = mkstemp(name);
    if (fd < 0) {
        return -1;
    }
    unlink(name);
    
    if (ftruncate(fd, size) < 0) {
        close(fd);
        return -1;
    }
    
    return fd;
}

static struct wl_buffer* create_buffer(int width, int height) {
    int stride = width * 4;
    int size = stride * height;
    
    int fd = create_shm_file(size);
    if (fd < 0) {
        printf("Failed to create shm file\n");
        return NULL;
    }
    
    void *data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        printf("Failed to mmap\n");
        close(fd);
        return NULL;
    }
    
    // Fill with bright green (ARGB format: 0xFF00FF00)
    uint32_t *pixels = (uint32_t*)data;
    for (int i = 0; i < width * height; i++) {
        pixels[i] = 0xFF00FF00; // Bright green
    }
    
    struct wl_shm_pool *pool = wl_shm_create_pool(shm, fd, size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, 0, width, height, stride, WL_SHM_FORMAT_ARGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);
    
    printf("Created green buffer %dx%d\n", width, height);
    return buffer;
}

int main() {
    printf("🟢 Starting green screen Wayland client...\n");
    
    display = wl_display_connect(NULL);
    if (!display) {
        printf("Failed to connect to Wayland display\n");
        return 1;
    }
    printf("Connected to Wayland display\n");
    
    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);
    wl_display_roundtrip(display);
    
    if (!compositor || !shm || !shell) {
        printf("Missing required Wayland interfaces\n");
        printf("compositor: %p, shm: %p, shell: %p\n", compositor, shm, shell);
        return 1;
    }
    
    surface = wl_compositor_create_surface(compositor);
    shell_surface = wl_shell_get_shell_surface(shell, surface);
    wl_shell_surface_set_toplevel(shell_surface);
    printf("Created surface and shell surface\n");
    
    buffer = create_buffer(800, 600);
    if (!buffer) {
        printf("Failed to create buffer\n");
        return 1;
    }
    
    wl_surface_attach(surface, buffer, 0, 0);
    wl_surface_commit(surface);
    printf("🟢 Green screen displayed\n");
    
    // Keep the green screen displayed for 30 seconds
    for (int i = 0; i < 300; i++) {  
        wl_display_dispatch_pending(display);
        wl_surface_damage(surface, 0, 0, 800, 600);
        wl_surface_commit(surface);
        usleep(100000); // 0.1 second
        
        if (i % 50 == 0) {
            printf("🟢 Green screen active (%d/300)\n", i);
        }
    }
    
    printf("🟢 Green screen client finished\n");
    wl_display_disconnect(display);
    return 0;
}