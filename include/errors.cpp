#include "errors.hpp"

exception_::exception_(const std::string msg): message(msg){}
const char* exception_::what() const noexcept {return message.c_str();}

index_out_of_range::index_out_of_range(): exception_("index out of range"){}
index_out_of_range::index_out_of_range(std::string msg): exception_(msg){}


invalid_argument::invalid_argument(): exception_("invalid argument"){}
invalid_argument::invalid_argument(std::string msg): exception_(msg){}

null_ptr::null_ptr(): exception_("null pointer"){}
null_ptr::null_ptr(std::string msg): exception_(msg){}

size_mismatch::size_mismatch(): exception_("size mismatch"){}
size_mismatch::size_mismatch(std::string msg): exception_(msg){}

empty_container::empty_container(): exception_("container is empty"){}
empty_container::empty_container(std::string msg): exception_(msg){}

not_found::not_found():exception_("element not found"){}
not_found::not_found(std::string msg): exception_(msg){}

iterator_out_of_range::iterator_out_of_range(): exception_("iterator out of range"){}
iterator_out_of_range::iterator_out_of_range(std::string msg): exception_(msg){}

