#pragma once
#include "SimpleMath.h"

#include <boost/uuid/uuid_serialize.hpp>

using namespace DirectX::SimpleMath;

namespace boost
{
	namespace serialization
	{
		template<class Archive>
		inline void serialize(
			Archive& ar,
			Vector3& t,
			const unsigned int file_version
		)
		{
			ar& t.x;
			ar& t.y;
			ar& t.z;
		}

		template<class Archive>
		inline void serialize(
			Archive& ar,
			Vector2& t,
			const unsigned int file_version
		)
		{
			ar& t.x;
			ar& t.y;
		}

		template<class Archive>
		inline void serialize(
			Archive& ar,
			Vector4& t,
			const unsigned int file_version
		)
		{
			ar& t.x;
			ar& t.y;
			ar& t.z;
			ar& t.w;
		}

	}
}