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
    bool InitializeBuffers(ID3D11Device* device);
    void CleanupBuffers();
    void RenderBuffers(ID3D11DeviceContext* context);
	
    ID3D11Buffer* mVertexBuffer = nullptr;
    ID3D11Buffer* mIndexBuffer = nullptr;
    int mVertexCount= 0, mIndexCount = 0;
};
