// jajson.h header file
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

#define long long int
#define JSON_NULL_VALUE 0

/**
project level comments:
- MAJOR-TODO => means application breaking things that are not handled yet.
- TODO => things that should be addressed but may be optional.
 */

/*
 * NOTES for jajson.h header file
 *
 * This header file will
 */

// ================== JSON RELATED START =================
// TODO: for easy contruction of json types, we will use variadic functions in c for json objects and json arrays

/**
 * \brief enum type defining possible json types
 */
typedef enum json_types_s
{
    JSON_STRING = 0,
    JSON_INT = 1,   // NOTE: the json number type is split into two types => int and float
    JSON_FLOAT = 2, // NOTE: the json number type is split into two types => int and float
    JSON_BOOL = 3,
    JSON_NULL = 4,
    JSON_OBJECT = 5,
    JSON_ARRAY = 6
} json_types_t;

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
json_value_t build_json_float(double float_v);
json_value_t build_json_bool(bool bool_v);
json_value_t build_json_null();
json_value_t build_json_object(int n_args, ...);
json_value_t build_json_array(int n_args, ...);
//===== END BUILD JSON INIT =====

//===== PRINT JSON INIT =====
static void print_tab_helper(int tab_level);
void print_json_string(json_string_t json_string, int tab_level, bool append_comma, bool is_object_value);
void print_json_int(json_int_t json_int, int tab_level, bool append_comma, bool is_object_value);
void print_json_float(json_float_t json_float, int tab_level, bool append_comma, bool is_object_value);
void print_json_bool(json_bool_t json_bool, int tab_level, bool append_comma, bool is_object_value);
void print_json_null(int tab_level, bool append_comma, bool is_object_value);
void print_json_object(json_object_t *json_object, int tab_level, bool append_comma, bool is_object_value);
void print_json_array(json_array_t *json_array, int tab_level, bool append_comma, bool is_object_value);

// All encompassing recursive function that users will interface with
static void print_json_value_helper(json_value_t json_value, int tab_level, bool append_comma, bool is_object_value);
void print_json_value(json_value_t json_value); // TODO: does this need to be a pointer?
//===== END PRINT JSON INIT =====

//===== SERIALIZE/DESERIALIZE JSON INIT =====
char *dump_json(json_value_t *json_value); // TOOD: just reuse print json logic. => determine string size by iterating once beforehand

// Helper functions for loading JSON
static char* skip_white_space(char *json); // used in load_json to skip white space in json data
static bool is_valid_json_number_char(char c);
static size_t find_string_size(char *in, char quote_style);
static char* read_string(char *in, char **out, size_t size, char quote_style);
static char* read_json_string(char *json, json_value_t *json_parsed, char quote_style);
static char* read_json_number(char *json, json_value_t *json_parsed);
static char* read_json_object(char *json, json_value_t *json_parsed);
static char* read_json_array(char *json, json_value_t *json_parsed);

static char* load_json_helper(char *json, json_value_t *json_parsed);
json_value_t* load_json(char *json);

void free_json(json_value_t *json_parsed); // Used for freeing allocated heap memory used to load json
//===== END SERIALIZE/DESERIALIZE JSON INIT =====

//===== BUILD JSON IMPLEMENTATION =====
/**
 * \brief Function to build a json string
 * \param[in] string_v[] string
 *
 * \return constructed json value
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
 * \brief Function to build a json integer
 * \param[in] int_v integer
 *
 * \return constructed json value
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
 * \brief Function to build a json floating value
 * \param[in] float_v floating point value
 *
 * \return constructed json value
 */
json_value_t build_json_float(double float_v)
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
 * \brief Function to build a json boolean
 * \param[in] bool_v boolean value
 *
 * \return constructed json value
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
 * \brief Function to build a json null
 *
 * \return constructed json value
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
 * \param[in] n_args number of key-value pairs in variadic function.
 *
 * \param[in] ... variable number of arguments describing key-value pairs describing
 * a key-value pair in a json object. Every pair of arguments in the variadic function
 * should follow the format: <char*>, <json_value_t>.
 *
 * \return constructed json value
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

        json_value_t *value = (json_value_t *) calloc(1, sizeof(json_value_t)); // get value from variadic list
        *value = va_arg(ap, json_value_t);

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
 * \brief builds a json array from many json_value_t values.
 *
 * \param[in] n_args number of json_value_t values in variadic function.
 *
 * \param[in] ... variable number of arguments describing each element in a json array. Each element in the
 * variadic array should be of the type <json_value_t>
 *
 * \return constructed json value
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
 * \brief Helper function to print tabs for formatting json indentation
 * 
 * \param[in] tab_level number of tabs to add
 */
static void print_tab_helper(int tab_level)
{
    for (int i = 0; i < tab_level; ++i)
    {
        printf("    ");
    }
}

/**
 * \brief Function to print a json string. Mainly used as a helper function.
 * 
 * \param[in] json_string json string to be printed
 * \param[in] tab_level number of tabs to ident the printed string by
 * \param[in] append_comma boolean to show if a comma should be added after string
 * \param[in] is_object_value tabs will be prepended if current json value is a json object value
 */
void print_json_string(json_string_t json_string, int tab_level, bool append_comma, bool is_object_value)
{
    if (!is_object_value) print_tab_helper(tab_level);
    printf("\"%s\"%s", 
        json_string.value, 
        append_comma ? ",\n" : "\n"
    );
}

/**
 * \brief Function to print a json integer. Mainly used as a helper function.
 * 
 * \param[in] json_int json integer to be printed
 * \param[in] tab_level number of tabs to ident the printed string by
 * \param[in] append_comma boolean to show if a comma should be added after string
 * \param[in] is_object_value tabs will be prepended if current json value is a json object value
 */
void print_json_int(json_int_t json_int, int tab_level, bool append_comma, bool is_object_value)
{
    if (!is_object_value) print_tab_helper(tab_level);
    printf("%ld%s", 
        json_int.value,
        append_comma ? ",\n" : "\n"
    );
}

/**
 * \brief Function to print a json float. Mainly used as a helper function.
 * 
 * \param[in] json_float json float to be printed
 * \param[in] tab_level number of tabs to ident the printed string by
 * \param[in] append_comma boolean to show if a comma should be added after string
 * \param[in] is_object_value tabs will be prepended if current json value is a json object value
 */
void print_json_float(json_float_t json_float, int tab_level, bool append_comma, bool is_object_value)
{
    if (!is_object_value) print_tab_helper(tab_level);
    printf("%f%s", 
        json_float.value,
        append_comma ? ",\n" : "\n"
    );
}

/**
 * \brief Function to print a json bool. Mainly used as a helper function.
 * 
 * \param[in] json_bool json bool to be printed
 * \param[in] tab_level number of tabs to ident the printed string by
 * \param[in] append_comma boolean to show if a comma should be added after string
 * \param[in] is_object_value tabs will be prepended if current json value is a json object value
 */
void print_json_bool(json_bool_t json_bool, int tab_level, bool append_comma, bool is_object_value)
{
    if (!is_object_value) print_tab_helper(tab_level);
    printf("%s%s", 
        json_bool.value ? "true" : "false",
        append_comma ? ",\n" : "\n"
    );
}

/**
 * \brief Function to print a json null. Mainly used as a helper function.
 * 
 * \param[in] json_null json null to be printed
 * \param[in] tab_level number of tabs to ident the printed string by
 * \param[in] append_comma boolean to show if a comma should be added after string
 * \param[in] is_object_value tabs will be prepended if current json value is a json object value
 */
void print_json_null(int tab_level, bool append_comma, bool is_object_value)
{
    if (!is_object_value) print_tab_helper(tab_level);
    printf("null%s", append_comma ? ",\n" : "\n");
}

/**
 * \brief Function to print a json object. Mainly used as a helper function.
 * 
 * \param[in] json_object json object to be printed
 * \param[in] tab_level number of tabs to ident the printed string by
 * \param[in] append_comma boolean to show if a comma should be added after string
 * \param[in] is_object_value tabs will be prepended if current json value is a json object value
 */
void print_json_object(json_object_t *json_object, int tab_level, bool append_comma, bool is_object_value)
{
    if (!is_object_value) print_tab_helper(tab_level);
    // TODO: need a way to maintain tab state to know how many tabs to insert
    // traverse linkedlist
    json_object_t *p;
    p = json_object;

    printf("{\n"); tab_level++;
    while (p != NULL)
    {
        bool append_value_comma = true;
        // print key
        print_tab_helper(tab_level);
        printf("\"%s\": ", p->key); // print colon to separate key and value

        // print value
        if (p->next == NULL) append_value_comma = false;
        print_json_value_helper(*(p->value), tab_level,
        append_value_comma, true
        );
        p = p->next;
    }

    print_tab_helper(--tab_level);
    printf("}%s", append_comma ? ",\n" : "\n");
}

/**
 * \brief Function to print a json array. Mainly used as a helper function.
 * 
 * \param[in] json_array json array to be printed
 * \param[in] tab_level number of tabs to ident the printed string by
 * \param[in] append_comma boolean to show if a comma should be added after string
 * \param[in] is_object_value tabs will be prepended if current json value is a json object value
 */
void print_json_array(json_array_t *json_array, int tab_level, bool append_comma, bool is_object_value)
{
    if (!is_object_value) print_tab_helper(tab_level);
    // traverse linkedlist
    json_array_t *p;
    p = json_array;

    printf("[\n"); tab_level++;
    while (p != NULL)
    {
        bool append_value_comma = true;

        // print value
        if (p->next == NULL) append_value_comma = false;
        print_json_value_helper(*(p->value), tab_level, append_value_comma, false
        );
        p = p->next;
    }

    print_tab_helper(--tab_level);
    printf("]%s", append_comma ? ",\n" : "\n");
}

/**
 * \brief Helper function to print a json value.
 * 
 * \param[in] json_value json value to be printed
 * \param[in] tab_level number of tabs to ident the printed string by
 * \param[in] append_comma boolean to show if a comma should be added after string
 * \param[in] is_object_value tabs will be prepended if current json value is a json object value
 */
static void print_json_value_helper(json_value_t json_value, int tab_level, bool append_comma, bool is_object_value)
{    
    switch (json_value.type)
    {
        case JSON_STRING:
            print_json_string(json_value.value->string, tab_level, append_comma, is_object_value);
            break;

        case JSON_INT:
            print_json_int(json_value.value->integer, tab_level, append_comma, is_object_value);
            break;

        case JSON_FLOAT:
            print_json_float(json_value.value->floating, tab_level, append_comma, is_object_value);
            break;

        case JSON_BOOL:
            print_json_bool(json_value.value->boolean, tab_level, append_comma, is_object_value);
            break;

        case JSON_NULL:
            print_json_null(tab_level, append_comma, is_object_value);
            break;

        case JSON_OBJECT:
            print_json_object(json_value.value->object, tab_level, append_comma, is_object_value);
            break;

        case JSON_ARRAY:
            print_json_array(json_value.value->array, tab_level, append_comma, is_object_value);
            break;
    };
}

/**
 * \brief User facing function to print a json value.
 * 
 * \param[in] json_value json value to be printed
 */
void print_json_value(json_value_t json_value)
{    
    print_json_value_helper(json_value, 0, false, false);
}
//===== END PRINT JSON IMPLEMENTATION =====

/**
 * \brief json serializer in jajson.h
 *
 * \param[in] json_value: json_value_t pointer to access json structured data
 *
 * \returns string containing json data in string format
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
 * \brief Helper function to different types of white space in string
 * 
 * \param[in] json input string
*
 * \return remaining string after skipping white space characters
 */
static char* skip_white_space(char *json) 
{
    while (*json == ' ' || *json == '\t' || *json == '\n') {
        json++;
    }

    return json;
}

/**
 * \brief Helper function to find the size of a string that is being parsed
 * 
 * \param[in] in input string
 * \param[in] quote_style quote type the json string is enclosed with
 *
 * \return size of first string found in input
 */
static size_t find_string_size(char *in, char quote_style)
{
    size_t size = 0;

    bool escaped = false;
    // Skip '"' or '\''
    in++; 

    // if we reach the end quote and 
    while (*in != quote_style || escaped) 
    {
        if (*in == '\\')
        {
          escaped = true;
          in++;
          continue;
        }

        size++;
        in++;

        if (escaped)
            escaped = false;
    }
    in++;

    return size + 1; // add one for null termination character
}

/**
 * \brief Helper function to read a json string from input
 * 
 * \param[in] in input string
 * \param[in] out address to a pointer that stores the parsed string
 * \param[in] size size of string to be parsed
 * \param[in] quote_style quote type the string to be parsed will be enclosed by
 *
 * \return remaining input string after reading the first string from the input string
 */
static char* read_string(char *in, char **out, size_t size, char quote_style)
{
    bool escaped = false;
    char *string = (char *) malloc(size); // allocate max size, TODO: figure out better way to do this
    char *temp = string;
    // Skip '"' or '\''
    in++; 

    // if we reach the end quote and 
    while (*in != quote_style || escaped) 
    {
        if (*in == '\\')
        {
          escaped = true;
          in++;
          continue;
        }

        *temp++ = *in++;

        if (escaped)
            escaped = false;
    }
    in++;
    
    *temp = '\0';
    *out = string;

    return in;
}

/**
 * \brief Function to read a json string
 *
 * \param[in] json input string
 * \param[in] json_parsed resultant json value to store parsed string
 * \param[in] quote_style quote type the string to be parsed will be enclosed by
 *
 * \return remaining input string after parsing first json string found
 */
static char* read_json_string(char *json, json_value_t *json_parsed, char quote_style)
{
    json_element_t *json_element = (json_element_t *)calloc(1, sizeof(json_element_t));
    size_t string_size = find_string_size(json, quote_style);
    char *string;

    json = read_string(json, &string, string_size, quote_style);
    
    // printf("String that is read: %s\n", string);

    json_string_t json_string;
    json_string.value = string;
    json_string.size = string_size;

    json_element->string = json_string;

    json_parsed->value = json_element;

    // MAJOR-TODO: what if json string is incorrect and escaped is true after leaving? or even worse, loop never ends
    // if (escaped)
    // {
    //     // todo: print error, force uncontinueable error?
    //     fprintf(stderr, "ERROR: Unable to read improper JSON string");
    // }

    return json; // return json to continue parsing
}

/**
 * \brief Helper function to determine if a character is a valid character in a json number
 *
 * \param[in] c character in question
 *
 * \return true if c is a valid json number character, false otherwise
 */
static bool is_valid_json_number_char(char c)
{
    return isdigit(c) || c == 'e' || c == 'E' || c == '.';
}

/**
 * \brief Function to read a json number
 *
 * \param[in] json input string
 * \param[in] json_parsed resultant json value to store parsed integer or floating point value
 *
 * \return remaining input string after parsing first json number found
 */
static char* read_json_number(char *json, json_value_t *json_parsed)
{
    json_element_t *json_element = (json_element_t *) calloc(1, sizeof(json_element_t));

    // TODO: add support for E, e in json values?
    // while string that is curretnly being read
    bool is_float = false;
    long is_negative = 1;
    int floating_count = 0;
    long integer = 0;

    bool use_scientific_notation = false;
    long exponent = 0;

    // negative check
    if (*json == '-') 
    {
        is_negative = -1;
        json++;
    }

    while (is_valid_json_number_char(*json))
    {
        if (*json == '.')
        {
            is_float = true;
            json++;
            continue;
        }
        
        if (*json == 'e' || *json == 'E')
        {
            use_scientific_notation = true;
            json++;
            continue;
        }

        if (is_float && !use_scientific_notation) 
        {
            floating_count -= 1;
        }

        if (use_scientific_notation)
        {
            exponent *= 10;
            exponent += *json++ - '0';
        } else 
        {
            integer *= 10;
            integer += *json++ - '0';
        }
    }

    // apply negative
    integer *= is_negative;

    if (is_float) 
    {
        if (use_scientific_notation) 
        {
            floating_count += exponent;
        }

        double float_value = (double) integer * pow(10, floating_count);
        json_float_t json_float;
        json_float.value = float_value;

        json_element->floating = json_float;
        
        json_parsed->type = JSON_FLOAT;
        json_parsed->value = json_element;
    } else 
    {
        if (use_scientific_notation) 
        {
            integer = integer * pow(10, exponent);
        }

        json_int_t json_int;
        json_int.value = integer;

        json_element->integer = json_int;

        json_parsed->type = JSON_INT;
        json_parsed->value = json_element;
    }

    return json;
}


/**
 * \brief Function to read a json object
 *
 * \param[in] json input string
 * \param[in] json_parsed resultant json value to store parsed json object
 *
 * \return remaining input string after parsing first json object found
 */
static char* read_json_object(char *json, json_value_t *json_parsed)
{
    json_element_t *json_element = (json_element_t *) calloc(1, sizeof(json_element_t));
    json_object_t *json_object = NULL;
    json++; // skip { character

    while (*json != '}')
    {
        json = skip_white_space(json);
        // read in the value for the string key
        char *key;
        if (*json == '"')
        {
            size_t string_size = find_string_size(json, '"');
            json = read_string(json, &key, string_size, '"');
        } else if (*json == '\'')
        {
            size_t string_size = find_string_size(json, '\'');
            json = read_string(json, &key, string_size, '\'');
        }

        // printf("json current: %c\n", *json);

        // Skip possible space between key and colon
        json = skip_white_space(json);
        json++; // skip colon value
        json = skip_white_space(json);

        json_value_t *value = (json_value_t*) calloc(1, sizeof(json_value_t));
        json = load_json_helper(json, value);

        // Extend json object linked list
        json_object_t *temp = (json_object_t*) calloc(1, sizeof(json_object_t));
        temp->key = key;
        temp->value = value;
        temp->next = json_object;

        json_object = temp;

        // Skip possible space between key and comma
        json = skip_white_space(json);
        if (*json == ',')
        {
            json++; // skip comma value if it exists
        }
        json = skip_white_space(json);
    }
    json++;
    
    json_element->object = json_object;
    json_parsed->value = json_element;

    return json;
}

/**
 * \brief Function to read a json array
 *
 * \param[in] json input string
 * \param[in] json_parsed resultant json value to store parsed json array
 *
 * \return remaining input string after parsing first json array found
 */
static char* read_json_array(char *json, json_value_t *json_parsed)
{
    json_element_t *json_element = (json_element_t *) calloc(1, sizeof(json_element_t));
    json_array_t *json_array = NULL;
    json++; // skip [ character

    while (*json != ']')
    {
        json = skip_white_space(json);

        json_value_t *value = (json_value_t*) calloc(1, sizeof(json_value_t));
        json = load_json_helper(json, value);

        // Extend json object linked list
        json_array_t *temp = (json_array_t*) calloc(1, sizeof(json_array_t));
        temp->value = value;
        temp->next = json_array;

        json_array = temp;

        // Skip possible space between key and comma
        json = skip_white_space(json);
        if (*json == ',')
        {
            json++; // skip comma value if it exists
        }
        json = skip_white_space(json);
    }
    json++;
    
    json_element->array = json_array;
    json_parsed->value = json_element;

    return json;
}


/**
 * \brief Helper function to read a json value
 *
 * \param[in] json input string
 * \param[in] json_parsed resultant json value to store parsed json value
 *
 * \return remaining input string after parsing first json object found
 */
static char* load_json_helper(char *json, json_value_t *json_parsed) 
{
    json = skip_white_space(json);

    /*
    Recursive descent parser
    - ", ' => leads to json string
    - n => leads to json null
    - t or f => leads to json bool
    - 0-9, - => leads to json int or float
    - { => leads to json object
    - [ => leads to json array
    */
    switch (*json)
    {
        case '"':
            json_parsed->type = JSON_STRING;
            // parse json string value
            json = read_json_string(json, json_parsed, '"');
            break;

        case '\'':
            json_parsed->type = JSON_STRING;
            // parse json string value that is enclosed by single quotes
            json = read_json_string(json, json_parsed, '\'');
            break;

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
        case '-':
            // parse json int / float
            json = read_json_number(json, json_parsed);
            break;

        case '{':
            json_parsed->type = JSON_OBJECT;
            // parse json object
            json = read_json_object(json, json_parsed);
            break;

        case '[':
            json_parsed->type = JSON_ARRAY;
            // parse json_array
            json = read_json_array(json, json_parsed);
            break;
        default:
            // check for json null
            // check for json boolean (true and false)
            if (*json == 't' && *(json + 1) == 'r' && *(json + 2) == 'u' && *(json + 3) == 'e')
            {
                json_parsed->type = JSON_BOOL;
                *json_parsed = build_json_bool(true);
                json += 4;
            }
            else if (*json == 'f' && *(json + 1) == 'a' && *(json + 2) == 'l' && *(json + 3) == 's' && *(json + 4) == 'e')
            {
                json_parsed->type = JSON_BOOL;
                *json_parsed = build_json_bool(false);
                json += 5;
            }
            else if (*json == 'n' && *(json + 1) == 'u' && *(json + 2) == 'l' && *(json + 3) == 'l')
            {
                json_parsed->type = JSON_NULL;
                *json_parsed = build_json_null();
                json += 4;
            }
            break;
    }

    return json;
}

/**
 * \brief json parser (deserializer) in jajson.h
 *
 * \param[in] json: input string that represents json data
 *
 * \returns json_value_t variable containing json data represented
 * using jajson.h defined json structs, enums, and unions
 */
json_value_t* load_json(char *json)
{
    // NOTE: only allocates memory to direct descendents
    json_value_t *json_value = (json_value_t*) calloc(1, sizeof(json_value_t));

    json = load_json_helper(json, json_value);
    
    return json_value;
}

/**
 * returns true if the freeing succeeded
 */
void free_json(json_value_t *json_parsed) {
    if (json_parsed->type == JSON_ARRAY) {
        json_array_t *p = json_parsed->value->array;
        while (p != NULL) {
            json_array_t *temp = p->next;
            free_json(p->value);
            p = temp;
        }
    } else if ( json_parsed->type == JSON_OBJECT) {
        json_object_t *p = json_parsed->value->object;
        while (p != NULL) {
            json_object_t *temp = p->next;
            free_json(p->value);
            p = temp;
        }       
    }

    free(json_parsed->value);
    free(json_parsed);
}
