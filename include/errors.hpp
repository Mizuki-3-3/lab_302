#pragma once

#include <exception>
#include <string>

class exception_ : public std::exception {
private:
    std::string message;
public:
    exception_(const std::string msg);
    const char* what() const noexcept override;
};

class index_out_of_range: public exception_{
public:
    index_out_of_range();
    index_out_of_range(std::string msg);
};

class invalid_argument: public exception_{
public:
    invalid_argument();
    invalid_argument(std::string msg);
};

class null_ptr: public exception_{
public:
    null_ptr();
    null_ptr(std::string msg);
};

class size_mismatch: public exception_{
public:
    size_mismatch();
    size_mismatch(std::string msg);
};

class empty_container: public exception_{
public:
    empty_container();
    empty_container(std::string msg);
};

class not_found: public exception_{
public:
    not_found();
    not_found(std::string msg);
};

class iterator_out_of_range: public exception_{
public:
    iterator_out_of_range();
    iterator_out_of_range(std::string msg);
};
