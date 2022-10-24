#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

class Model
{
private:
    struct VertexType
    {
        XMFLOAT3 Position;
        XMFLOAT4 Color;
    };
public:
    Model(ID3D11Device* device);
    ~Model();
    
	void Cleanup();
	void Render(ID3D11DeviceContext* context);

	int GetIndexCount() const;
private:
    ID3D11Buffer* mVertexBuffer;
    ID3D11Buffer* mIndexBuffer;
	int mVertexCount, mIndexCount;
};
