#ifndef VARIANT_H
#define VARIANT_H

#define join2(a, b) a##b
#define join3(a, b, c) a##b##c
#define join4(a, b, c, d) a##b##c##d

#define gen_enum_name(name, variant_name, type) join4(variant_, variant_name, _type_, name)
#define gen_enum(name, variant_name, type) gen_enum_name(name, variant_name, type),
#define gen_var(name, variant_name, type) type name;

#define define_variant(variant_name, variant_types)\
  typedef enum {\
    variant_types(gen_enum)\
  } join3(variant_, variant_name, _type);\
  struct join2(variant_, variant_name) {\
    join3(variant_, variant_name, _type) type;\
    union {\
      variant_types(gen_var)\
    };\
  };

#define forward_dec_variant(variant_name)\
  typedef struct join2(variant_, variant_name) join2(variant_, variant_name)

#define make_variant(variant_name, variant_type_name, val)\
  (join2(variant_, variant_name)){.variant_type_name = val, .type = gen_enum_name(variant_type_name, variant_name, variant_type)}

#define make_variant_alloc(variant_name, alloc)\
  (join2(variant_, variant_name)*) alloc(sizeof(join2(variant_, variant_name)))\

// #define match_variant(variant, variant_type)\
//   if (variant.type == gen_enum_name(variant_type, variant_name)) {}

#define match_variant(variant, variant_name, ...)\
{\
  join2(variant_, variant_name) v = variant;\
  switch (v.type) {\
    __VA_ARGS__\
  }\
}

#define variant_case(variant_name, variant_type_name, okbody)\
  case gen_enum_name(variant_type_name, variant_name, variant_type): okbody break;\

#define variant_default(defaultbody)\
  default: defaultbody

#endif
