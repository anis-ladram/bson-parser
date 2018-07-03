#include "bson.hh"

#include <cstdint>
#include <functional>
#include <ios>
#include <istream>
#include <ostream>
#include <string>

#include "bson-base.hh"
#include "bson-document-base.hh"
#include "bson-generic.hh"
#include "bson-utils.hh"

bson::bson(std::istream& s)
{
    while (s.peek() != std::istream::traits_type::eof())
        docs_.emplace_back(bson_document(s));
}

#define BSON_ELEM(Type) \
{ Type::id(), [] (std::istream& s) { return std::make_shared<Type>(s); } }

auto bson_factory(char id)
{
    static const std::unordered_map<
        char,
        std::function<std::shared_ptr<bson_element>(std::istream&)>
    > map {
        BSON_ELEM(bson_double),
        BSON_ELEM(bson_document),
        BSON_ELEM(bson_boolean),
        BSON_ELEM(bson_int32),
        BSON_ELEM(bson_timestamp),
        BSON_ELEM(bson_int64)
    };

    return map.at(id);
}

void bson::dump(std::ostream& s) const
{
    for (const auto& doc: docs_)
        s << doc << std::endl;
}
