//-----------------------------------------------------------
// File: Sprite.cpp
//-----------------------------------------------------------
#include "STD.h"
#include "Sprite.h"
#include "GraphicsDeviceManager.h"

Sprite::Sprite()
{
	m_pTex = NULL;
	//m_pSprite = NULL;
}

Sprite::~Sprite()
{
	//if( m_pSprite ) { m_pSprite->Release(); m_pSprite = NULL; }
}

void Sprite::Initialize( const String& textureName )
{
	// Get the texture from the texture manager. If a null
	// value is returned then stop the program.
	/*m_pTex = TextureManager::GetInstance().GetTexture(textureName);
	ASSERT( m_pTex != NULL );*/

	// Get the sprite size
	/*D3DSURFACE_DESC desc;
	ZeroMemory( &desc, sizeof( desc ) );
	m_pTex->GetLevelDesc( 0, &desc );
	spriteSize = MyVector2( desc.Width, desc.Height );*/

	/*HRESULT hr = D3DXCreateSprite( GraphicsDeviceManager::GetInstance().GetDevice(), &m_pSprite );
	ASSERT( hr != E_FAIL );*/
}

void Sprite::Draw( const MyVector2& _pos )
{
	//m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );

	//m_pSprite->Draw( m_pTex, 
	//	NULL, // Source Rect
	//	&D3DXVECTOR3( spriteSize.x / 2, spriteSize.y / 2, 0 ), // Center
	//	&D3DXVECTOR3( _pos.x, _pos.y, 0 ),  // Position
	//	D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) ); // Color

	//m_pSprite->End();
}

void Sprite::Draw( const MyVector2& _pos, const MyVector2& _size )
{
	Matrix transform;
	Matrix scale;
	Matrix translation;

	//D3DXMatrixIdentity( &transform );
	//D3DXMatrixIdentity( &scale);
	//D3DXMatrixIdentity( &translation );

	//D3DXMatrixScaling( &scale, _size.x, _size.y, 1 );
	//D3DXMatrixTranslation( &translation, _pos.x, _pos.y, 1 );

	//D3DXMatrixMultiply( &transform, &scale, &translation );

	//m_pSprite->SetTransform( &transform );

	//m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );

	//m_pSprite->Draw( m_pTex, 
	//	NULL, // Source Rect
	//	&D3DXVECTOR3( spriteSize.x / 2, spriteSize.y / 2, 0 ), // Center
	//	NULL,  // Position
	//	D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) ); // Color

	//m_pSprite->End();
}

void Sprite::Draw( const MyVector2& _pos, const MyVector2& _size, const float alpha )
{
	Matrix transform;
	Matrix scale;
	Matrix translation;

	//D3DXMatrixIdentity( &transform );
	//D3DXMatrixIdentity( &scale);
	//D3DXMatrixIdentity( &translation );

	//D3DXMatrixScaling( &scale, _size.x, _size.y, 1 );
	//D3DXMatrixTranslation( &translation, _pos.x, _pos.y, 1 );

	//D3DXMatrixMultiply( &transform, &scale, &translation );

	//m_pSprite->SetTransform( &transform );

	//m_pSprite->Begin( D3DXSPRITE_ALPHABLEND );

	//m_pSprite->Draw( m_pTex, 
	//	NULL, // Source Rect
	//	&D3DXVECTOR3( spriteSize.x / 2, spriteSize.y / 2, 0 ), // Center
	//	NULL,  // Position
	//	D3DXCOLOR( 1.0f, 1.0f, 1.0f, alpha ) ); // Color

	//m_pSprite->End();
}