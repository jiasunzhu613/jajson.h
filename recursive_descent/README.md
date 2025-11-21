# jajson.h 

Just Another blazingly fast JSON header. A header file containing serializer and parser for the de facto data exchange format over the web.

# Usage

Include the single file header file into your project! That's all!

# Roadmap

TODO (in order of rough priority):
- [x] Add check to fix string sizes on creation
    - [ ] Add size checks to all json types
- [x] add support for scientific notation in parsing integers and floats
- [x] add proper print json formatting
    - [ ] add variable print json formatting for different lengthed json objects and json arrays
- [ ] implement hashmap for json objects
- [ ] implement dynamic array for json array
~~- [ ] implement simd processing using intrinsics for deserialization~~

# Benchmarks
~222.5 MB/s throughput
