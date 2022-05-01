import c4.regen as regen
serialize = regen.ClassGenerator(
    name="serialzie",
    hdr_preamblee="""\
#include "boost/serialization/base_object.hpp"
#include "boost/serialization/access.hpp"'
""",
    hdr="""\
template<class Archive>
void {{type}}::serialize(Archive& ar, const unsigned int version)
{
    boost::serialization::void_cast_register<{{type}}, GameObjectComponent>(
        static_cast<{{type}}*>(NULL),
        static_cast<GameObjectComponent*>(NULL)
        );
    //ar& boost::serialization::base_object<GameObjectComponent>(*this);
    {% for member in members %}
    ar& {{member.name}}
    {% endfor %}
}
"""
)

writer = regen.ChunkWriterGenFile()

# -----------------------------------------------------------------------------
if __name__ == "__main__":
    regen.run(writer, None, [serialize])