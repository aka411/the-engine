//#define STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "../include/stb_image.h"
//#include "../include/stb_image_write.h"

/*The problem this file is used to solve is that both tingltf and 
my GPUTextureManager use stb, since stb is single header libary we are centralising the
implementation so as to avoid redefinitions*/


/*I have commented out stb headers here cause tiny gltf itself has stb libraries inside */




#define NLOHMANN_JSON_IMPLEMENTATION
#include "nlohmann/json.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"



#define TINYGLTF_IMPLEMENTATION

#define TINYGLTF_NO_INCLUDE_JSON
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE

#include "tinygltf/tiny_gltf.h"