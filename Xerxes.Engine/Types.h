#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_serialize.hpp>

#define GameResourceId XID

namespace Xerxes
{
	namespace Engine
	{
		typedef boost::uuids::uuid XID;

		/*template<class Archive>
		inline void serialize(
			Archive& ar,
			XID& t,
			const unsigned int file_version
		)
		{
			ar& (boost::uuids::uuid)t;
		}*/

	}
}