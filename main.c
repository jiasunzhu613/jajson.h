#include "jajson.h"
#include <stdio.h>

int main() {
    // json_value_t json;

    // // casting malloc to json_element_t* type
    // json_element_t *element = (json_element_t*) malloc(sizeof(json_element_t)); // not using malloc here causes seg fault
    // printf("%lu\n", sizeof(element));

    // char json_string_value[] = "hello world";
    // printf("%lu\n", sizeof(json_string_value));
    // json_string_t json_string;
    // json_string.value = json_string_value;
    // printf("first layer assignment: %s\n", json_string.value);
    // json_string.size = sizeof(json_string_value);

    // element->string = json_string;
    // printf("second layer assignment: %s\n", element->string.value);

    // json.value = element;
    // json.type = JSON_STRING;

    // printf("good till here");

    // iterate:
    // printf("third layer assignment: %s\n", json.value->string.value);

    // free(json.value);

    // char string_v[] = "hello world";

    json_value_t json = build_json_string("hello world");
    print_json_value(json);

    // char string_v2[] = "hello world";

    // json_value_t json2 = build_json_string("hello world");

    // char string_v3[] = "hello world";

    // // json_value_t json3 = build_json_string(string_v3);
    // printf("third layer assignment: %s\n", json.value->string.value);
    // // json.value->string.value[2] = 's';

    // printf("maybe segfault here: %s\n", json.value->string.value);

    json_value_t json_object = build_json_object(5, 
        "hello", build_json_int(10), 
        "hello1", build_json_float(10.910108), 
        "hello2", build_json_bool(true), 
        "hello3", build_json_null(), 
        "new object", build_json_object(2,
            "test1", build_json_string("hi testing"),
            "alsjfsak", build_json_int(10238192)
        ),
        "test", build_json_string("hi")
    );
    print_json_value(json_object);

    json_value_t json_array = build_json_array(5,
        build_json_string("hello world"),
        build_json_int(10),
        build_json_float(10.1),
        build_json_bool(false),
        build_json_null()
    );

    print_json_value(json_array);
    // printf("%p\n", &json_object);
    // printf("first element:: type: %d, key: %s, value: %s\n", json_object.type, json_object.value->object->key, json_object.value->object->value->value->string.value);

    return 0;
}