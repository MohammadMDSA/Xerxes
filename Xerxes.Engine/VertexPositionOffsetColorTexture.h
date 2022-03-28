#pragma once

using namespace DirectX;

namespace Xerxes
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace VertexTypes
			{

				struct VertexPositionOffsetColorTexture
				{
					VertexPositionOffsetColorTexture() = default;

					VertexPositionOffsetColorTexture(const VertexPositionOffsetColorTexture&) = default;
					VertexPositionOffsetColorTexture& operator=(const VertexPositionOffsetColorTexture&) = default;

					VertexPositionOffsetColorTexture(VertexPositionOffsetColorTexture&&) = default;
					VertexPositionOffsetColorTexture& operator=(VertexPositionOffsetColorTexture&&) = default;

					VertexPositionOffsetColorTexture(XMFLOAT3 const& iposition, XMFLOAT3 const& ioffset, XMFLOAT4 const& icolor, XMFLOAT2 const& itextureCoordinate) noexcept
						: position(iposition),
						offset(ioffset),
						color(icolor),
						textureCoordinate(itextureCoordinate)
					{ }

					VertexPositionOffsetColorTexture(FXMVECTOR iposition, FXMVECTOR ioffset, FXMVECTOR icolor, FXMVECTOR itextureCoordinate) noexcept
					{
						XMStoreFloat3(&this->position, iposition);
						XMStoreFloat3(&this->offset, ioffset);
						XMStoreFloat4(&this->color, icolor);
						XMStoreFloat2(&this->textureCoordinate, itextureCoordinate);
					}

					XMFLOAT3 position;
					XMFLOAT3 offset;
					XMFLOAT4 color;
					XMFLOAT2 textureCoordinate;

					static constexpr unsigned int InputElementCount = 4;
					static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
				};

			}
		}
	}
}
