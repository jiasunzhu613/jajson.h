// jajson.h header file
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#define long long int
#define JSON_NULL_VALUE 0

/*
 * NOTES for jajson.h header file
 *
 * This header file will
 */

// ================== JSON RELATED START =================
// TODO: for easy contruction of json types, we will use variadic functions in c for json objects and json arrays

typedef enum json_types_s json_types_t;
typedef struct json_string_s json_string_t;
typedef struct json_int_s json_int_t;
typedef struct json_float_s json_float_t;
typedef struct json_bool_s json_bool_t;
typedef struct json_null_s json_null_t;
typedef struct json_object_s json_object_t;
typedef struct json_array_s json_array_t;
typedef union json_element_s json_element_t;
typedef struct json_value_s json_value_t;

/**
 * \brief enum type defining possible json types
 */
enum json_types_s
{
    JSON_STRING = 0,
    JSON_INT = 1,   // NOTE: the json number type is split into two types => int and float
    JSON_FLOAT = 2, // NOTE: the json number type is split into two types => int and float
    JSON_BOOL = 3,
    JSON_NULL = 4,
    JSON_OBJECT = 5,
    JSON_ARRAY = 6
};

/**
 * \brief struct defining json string type
 */
struct json_string_s
{
    const char *value;
    size_t size;
};

/**
 * \brief struct defining json int type
 */
struct json_int_s
{
    long value;
    size_t size;
};

/**
 * \brief struct defining json float type
 */
struct json_float_s
{
    double value;
    size_t size;
} ;

/**
 * \brief struct defining json bool type
 */
struct json_bool_s
{
    bool value;
    size_t size;
};

/**
 * \brief struct defining json null type
 */
struct json_null_s
{
    short value; // will only contain the json_null value
    size_t size;
};

// TODO: key value pair, implement with linked list? then try hash table?
/**
 * \brief struct defining json object type
 */
struct json_object_s
{
    const char *key;
    struct json_value_s *value;
    struct json_object_s *next; // pointer to next element in linked list
};

/**
 * \brief struct defining json array type
 */
struct json_array_s
{
    struct json_value_s *value;
    struct json_array_s *next; // pointer to next element in linked list
};

/**
 * \brief union defining json element type. A json element in jajson.h defines the
 * possible values a json_value_t type can carry
 *
 * NOTE: Union is retrieved based on value name
 */
union json_element_s
{
    struct json_string_s string;
    struct json_int_s integer;
    struct json_float_s floating;
    struct json_bool_s boolean;
    struct json_null_s null;
    struct json_object_s *object;
    struct json_array_s *array;
};

/**
 * \brief struct defining json value type. json_value_t is the user-interfacing json type
 * for users
 */
struct json_value_s
{
    union json_element_s *value;
    enum json_types_s type;
};

// Function init (for function docstrings, see function implementation)
//===== BUILD JSON INIT =====
json_value_t build_json_string(const char string_v[]);
json_value_t build_json_int(int int_v);
json_value_t build_json_float(float float_v);
json_value_t build_json_bool(bool bool_v);
json_value_t build_json_null();
json_value_t build_json_object(int n_args, ...);
json_value_t build_json_array(int n_args, ...);
//===== END BUILD JSON INIT =====

//===== PRINT JSON INIT =====
void print_json_string(json_string_t json_string);
void print_json_int(json_int_t json_int);
void print_json_float(json_float_t json_float);
void print_json_bool(json_bool_t json_bool);
void print_json_null();
void print_json_object(json_object_t *json_object);
void print_json_array(json_array_t *json_array);

// All encompassing recursive function that users will interface with
void print_json_value(json_value_t json_value); // TODO: does this need to be a pointer?
//===== END PRINT JSON INIT =====

//===== SERIALIZE/DESERIALIZE JSON INIT =====
char *dump_json(json_value_t *json_value);

// Helper functions for loading JSON
char* skip_white_space(char *json); // used in load_json to skip white space in json data
char* read_json_string(char *json, json_value_t *json_parsed);
char* read_json_number(char *json, json_value_t *json_parsed);
// char* read_json_float(char *json, json_value_t *json_parsed);
char* read_json_bool(char *json, json_value_t *json_parsed);
char* read_json_null(char *json, json_value_t *json_parsed);
char* read_json_object(char *json, json_value_t *json_parsed);
char* read_json_array(char *json, json_value_t *json_parsed);

json_value_t *load_json(char *json);
//===== END SERIALIZE/DESERIALIZE JSON INIT =====

//===== BUILD JSON IMPLEMENTATION =====
/**
 * \brief
 */
json_value_t build_json_string(const char string_v[])
{
    // Build json_element_t
    json_element_t *json_element = (json_element_t *)calloc(1, sizeof(json_element_t));

    json_string_t json_string;
    json_string.value = strdup(string_v);
    json_string.size = strlen(string_v);

    json_element->string = json_string;

    // return json_value_t
    json_value_t json_value;
    json_value.value = json_element;
    json_value.type = JSON_STRING;

    return json_value;
}

/**
 * \brief
 */
json_value_t build_json_int(int int_v)
{
    // Build json_element_t
    json_element_t *json_element = (json_element_t *)calloc(1, sizeof(json_element_t));

    json_int_t json_int;
    json_int.value = int_v;
    json_int.size = sizeof(int_v);

    json_element->integer = json_int;

    // return json_value_t
    json_value_t json_value;
    json_value.value = json_element;
    json_value.type = JSON_INT;

    return json_value;
}

/**
 * \brief
 */
json_value_t build_json_float(float float_v)
{
    // Build json_element_t
    json_element_t *json_element = (json_element_t *)calloc(1, sizeof(json_element_t));

    json_float_t json_float;
    json_float.value = float_v;
    json_float.size = sizeof(json_float);

    json_element->floating = json_float;

    // return json_value_t
    json_value_t json_value;
    json_value.value = json_element;
    json_value.type = JSON_FLOAT;

    return json_value;
}

/**
 * \brief
 */
json_value_t build_json_bool(bool bool_v)
{
    // Build json_element_t
    json_element_t *json_element = (json_element_t *)calloc(1, sizeof(json_element_t));

    json_bool_t json_bool;
    json_bool.value = bool_v;
    json_bool.size = sizeof(json_bool);

    json_element->boolean = json_bool;

    // return json_value_t
    json_value_t json_value;
    json_value.value = json_element;
    json_value.type = JSON_BOOL;

    return json_value;
}

/**
 * \brief
 */
json_value_t build_json_null()
{
    // Build json_element_t
    json_element_t *json_element = (json_element_t *)calloc(1, sizeof(json_element_t));

    json_null_t json_null;
    json_null.value = JSON_NULL_VALUE;
    json_null.size = 0;

    json_element->null = json_null;

    // return json_value_t
    json_value_t json_value;
    json_value.value = json_element;
    json_value.type = JSON_NULL;

    return json_value;
}

/**
 * \brief builds a json object from many string, json_value_t key-value pairs.
 *
 * \param n_args number of key-value pairs in variadic function.
 *
 * \param ... variable number of arguments describing key-value pairs describing
 * a key-value pair in a json object. Every pair of arguments in the variadic function
 * should follow the format: <char*>, <json_value_t>.
 *
 */
json_value_t build_json_object(int n_args, ...)
{
    json_element_t *json_element = (json_element_t *) calloc(1, sizeof(json_element_t));
    json_object_t *json_object = NULL; // set initial linkedlist node as NULL

    // Parse variadic function arguments
    va_list ap;

    va_start(ap, n_args); // Collect variadic list

    // process args
    for (int i = 0; i < n_args ; ++i)
    {
        char *key = va_arg(ap, char *);                // get key from variadic list
        printf("Got key: %s\n", key);

        json_value_t *value = (json_value_t *) calloc(1, sizeof(json_value_t)); // get value from variadic list
        *value = va_arg(ap, json_value_t);
        printf("Got value: ");
        print_json_value(*value);
        printf("\n");

        // Build linkedlist
        json_object_t *temp = (json_object_t *) calloc(1, sizeof(json_object_t));
        temp->key = key;
        temp->value = value;
        temp->next = json_object;
        json_object = temp;
    }

    va_end(ap); // End variadic list

    json_element->object = json_object; // assign json_object to json_element

    json_value_t json_value;
    json_value.type = JSON_OBJECT;
    json_value.value = json_element; // access stops working here, some invalid memory acc here

    return json_value;
}

/**
 * \brief
 */
json_value_t build_json_array(int n_args, ...)
{
    json_element_t *json_element = (json_element_t *) calloc(1, sizeof(json_element_t));
    json_array_t *json_array = NULL;

    // Parse variadic function arguments
    va_list ap;

    va_start(ap, n_args); // Collect variadic list

    // process args
    for (int i = 0; i < n_args; ++i)
    {
        json_value_t *value = (json_value_t *) calloc(1, sizeof(json_value_t)); // get value from variadic list
        *value = va_arg(ap, json_value_t);

        // build linkedlist
        json_array_t *temp = (json_array_t *)calloc(1, sizeof(json_array_t));
        ;
        temp->value = value;
        temp->next = json_array;
        json_array = temp;
    }

    va_end(ap); // End variadic list

    json_element->array = json_array; // assign json_array to json_element

    json_value_t json_value;
    json_value.type = JSON_ARRAY;
    json_value.value = json_element;

    return json_value;
}
//===== END BUILD JSON IMPLEMENTATION=====

//===== PRINT JSON IMPLEMENTATION =====
/**
 *
 */
void print_json_string(json_string_t json_string)
{
    printf("\"%s\" ", json_string.value);
}

/**
 *
 */
void print_json_int(json_int_t json_int)
{
     printf("%ld ", json_int.value);
}

/**
 *
 */
void print_json_float(json_float_t json_float)
{
     printf("%f ", json_float.value);
}

/**
 *
 */
void print_json_bool(json_bool_t json_bool)
{
    printf("%s ", json_bool.value ? "true" : "false");
}

/**
 *
 */
void print_json_null()
{
    printf("null ");
}

/**
 * TODO: Fix this function for printing nested objects
 */
void print_json_object(json_object_t *json_object)
{
    // traverse linkedlist
    json_object_t *p;
    p = json_object;

    printf("{\n");
    while (p != NULL)
    {
        // print key
        printf("    \"%s\": ", p->key); // print colon to separate key and value

        // print value
        print_json_value(*(p->value));
        printf(",\n");
        p = p->next;
    }
    printf("}\n");
}

/**
 *
 */
void print_json_array(json_array_t *json_array)
{
    // traverse linkedlist
    json_array_t *p;
    p = json_array;

    printf("[\n");
    while (p != NULL)
    {
        // print value
        print_json_value(*(p->value));
        printf(",\n");
        p = p->next;
    }
    printf("]\n");
}

/**
 *
 */
void print_json_value(json_value_t json_value)
{
    switch (json_value.type)
    {
        case JSON_STRING:
            print_json_string(json_value.value->string);
            break;

        case JSON_INT:
            print_json_int(json_value.value->integer);
            break;

        case JSON_FLOAT:
            print_json_float(json_value.value->floating);
            break;

        case JSON_BOOL:
            print_json_bool(json_value.value->boolean);
            break;

        case JSON_NULL:
            print_json_null();
            break;

        case JSON_OBJECT:
            print_json_object(json_value.value->object);
            break;

        case JSON_ARRAY:
            print_json_array(json_value.value->array);
            break;
    };
}
//===== END PRINT JSON IMPLEMENTATION =====

/**
 * \brief json serializer in jajson.h
 *
 * \param[in] json_value: json_value_t pointer to access json structured data
 *
 * \returns c-style string containing json data in string format
 */
char* dump_json(json_value_t *json_value)
{
    /*
    issues:
    - how to load into c-string, make char array as big as possible?
    */
    return "hello";
}

/**
 * 
 */
char* skip_white_space(char *json) 
{
    while (*json == ' ' || *json == '\t' || *json == '\n') {
        json++;
    }

    return json;
}

/**
 * 
 */
char* read_json_string(char *json, json_value_t *json_parsed)
{
    bool escaped = false;
    char* string = (char *) malloc(5000); // allocate max size, TODO: figure out better way to do this

    // Skip '"' or '\''
    json++; 

    while (*json != '"') 
    {
        if (*json == '\\')
        {
            
        }

        *string++ = *json++;

        if (escaped)
            escaped = false;
    }
}

/**
 * 
 */
char* read_json_number(char *json, json_value_t *json_parsed)
{
    
}

/**
 * 
 */
char* read_json_bool(char *json, json_value_t *json_parsed)
{

}

/**
 * 
 */
char* read_json_null(char *json, json_value_t *json_parsed)
{

}

/**
 * 
 */
char* read_json_object(char *json, json_value_t *json_parsed)
{

}

/**
 * 
 */
char* read_json_array(char *json, json_value_t *json_parsed)
{

}

/**
 * \brief json parser (deserializer) in jajson.h
 *
 * \param[in] json: c-style string that represents json data
 *
 * \returns json_value_t variable containing json data represented
 * using jajson.h defined json structs, enums, and unions
 */
json_value_t* load_json(char *json)
{
    json_value_t *json_value = (json_value_t*) calloc(1, sizeof(json_value_t));

    json = skip_white_space(json);

    /*
    Recursive descent parser
    - " => leads to json string
    - n => leads to json null
    - t or f => leads to json bool
    -  
    */
    switch (*json)
    {
        case '"':
            json_value->type = JSON_STRING;
            // parse json string value
            json = read_json_string(json, json_value);
            break;

        case '\'':
            json_value->type = JSON_STRING;
            // parse json string value that is enclosed by single quotes
            break;

        // add support for +, -, ., E leading json int values?
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            json = read_json_number(json, json_value);
            // parse json int / float
            break;

        case '{':
            json_value->type = JSON_OBJECT;
            // parse json object
            json = read_json_object(json, json_value);
            break;

        case '[':
            json_value->type = JSON_ARRAY;
            // parse json_array
            json = read_json_array(json, json_value);
            break;
        default:
            // check for json null
            // check for json boolean (true and false)
            if (*json == 't' && *(json + 1) == 'r' && *(json + 2) == 'u' && *(json + 3) == 'e')
            {
                *json_value = build_json_bool(true);
                *json += 4;
            }
            else if (*json == 'f' && *(json + 1) == 'a' && *(json + 2) == 'l' && *(json + 3) == 's' && *(json + 4) == 'e')
            {
                *json_value = build_json_bool(false);
                *json += 5;
            }
            else if (*json == 'n' && *(json + 1) == 'u' && *(json + 2) == 'l' && *(json + 3) == 'l')
            {
                *json_value = build_json_null();
                *json += 4;
            }
    }
    

    
    return json_value;
}
