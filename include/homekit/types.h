#ifndef __HOMEKIT_TYPES_H__
#define __HOMEKIT_TYPES_H__

#include <stdbool.h>

typedef enum {
    homekit_format_bool,
    homekit_format_uint8,
    homekit_format_uint16,
    homekit_format_uint32,
    homekit_format_uint64,
    homekit_format_int,
    homekit_format_float,
    homekit_format_string,
    homekit_format_tlv,
    homekit_format_data
} homekit_format_t;

typedef enum {
    homekit_unit_none,
    homekit_unit_celsius,
    homekit_unit_percentage,
    homekit_unit_arcdegrees,
    homekit_unit_lux,
    homekit_unit_seconds
} homekit_unit_t;

typedef enum {
    homekit_permissions_paired_read = 1,
    homekit_permissions_paired_write = 2,
    homekit_permissions_notify = 4,
    homekit_permissions_additional_authorization = 8,
    homekit_permissions_timed_write = 16,
    homekit_permissions_hidden = 32
} homekit_permissions_t;

typedef enum {
    homekit_accessory_category_other = 1,
    homekit_accessory_category_bridge = 2,
    homekit_accessory_category_fan = 3,
    homekit_accessory_category_garage = 4,
    homekit_accessory_category_lightbulb = 5,
    homekit_accessory_category_door_lock = 6,
    homekit_accessory_category_outlet = 7,
    homekit_accessory_category_switch = 8,
    homekit_accessory_category_thermostat = 9,
    homekit_accessory_category_sensor = 10,
    homekit_accessory_category_security_system = 11,
    homekit_accessory_category_door = 12,
    homekit_accessory_category_window = 13,
    homekit_accessory_category_window_covering = 14,
    homekit_accessory_category_programmable_switch = 15,
    homekit_accessory_category_range_extender = 16,
    homekit_accessory_category_ip_camera = 17,
    homekit_accessory_category_video_door_bell = 18,
    homekit_accessory_category_air_purifier = 19,
} homekit_accessory_category_t;

struct _homekit_accessory;
struct _homekit_service;
struct _homekit_characteristic;
typedef struct _homekit_accessory homekit_accessory_t;
typedef struct _homekit_service homekit_service_t;
typedef struct _homekit_characteristic homekit_characteristic_t;

typedef struct _homekit_characteristic_change_callback {
    void (*function)(homekit_characteristic_t *characteristic, void *context);
    void *context;
    struct _homekit_characteristic_change_callback *next;
} homekit_characteristic_change_callback_t;

struct _homekit_characteristic {
    homekit_service_t *service;

    unsigned int id;
    const char *type;
    const char *description;
    union {
        bool bool_value;
        int int_value;
        float float_value;
        char *string_value;
        // tlv
        // data
    };
    homekit_format_t format;
    homekit_unit_t unit;
    homekit_permissions_t permissions;
    float *min_value;
    float *max_value;
    float *min_step;
    int *max_len;
    int *max_data_len;
    // valid-values
    // valid-values-range

    void *getter;
    void *setter;
    homekit_characteristic_change_callback_t *callbacks;
};

struct _homekit_service {
    homekit_accessory_t *accessory;

    unsigned int id;
    const char *type;
    bool hidden;
    bool primary;
    // linked
    homekit_characteristic_t *characteristics[8];
};

struct _homekit_accessory {
    unsigned int id;

    homekit_accessory_category_t category;
    int config_number;

    homekit_service_t *services[8];
};

#define HOMEKIT_ACCESSORY(...) \
    &(homekit_accessory_t) { \
        .config_number=1, \
        .category=homekit_accessory_category_other, \
        ##__VA_ARGS__ \
    }

#define HOMEKIT_SERVICE(_type, ...) \
    &(homekit_service_t) { .type=_type, ##__VA_ARGS__ }

#define HOMEKIT_CHARACTERISTIC(...) \
    &(homekit_characteristic_t) { \
        .type = 0, \
        .format = homekit_format_uint8, \
        .unit = homekit_unit_none, \
        .permissions = homekit_permissions_paired_read, \
        ##__VA_ARGS__ \
    }


void homekit_accessories_init(homekit_accessory_t **accessories);
homekit_characteristic_t *homekit_characteristic_find_by_id(homekit_accessory_t **accessories, int aid, int iid);

homekit_characteristic_t *homekit_characteristic_find_by_type(homekit_accessory_t **accessories, int aid, const char *type);

void homekit_characteristic_notify(homekit_characteristic_t *ch);
void homekit_characteristic_add_notify_callback(
    homekit_characteristic_t *ch,
    void (*callback)(homekit_characteristic_t *, void *),
    void *context
);
void homekit_characteristic_remove_notify_callback(
    homekit_characteristic_t *ch,
    void (*callback)(homekit_characteristic_t *, void *),
    void *context
);
void homekit_accessories_clear_notify_callbacks(
    homekit_accessory_t **accessories,
    void (*function)(homekit_characteristic_t *, void *),
    void *context
);
bool homekit_characteristic_has_notify_callback(
    homekit_characteristic_t *ch,
    void (*function)(homekit_characteristic_t *, void *),
    void *context
);


#endif // __HOMEKIT_TYPES_H__