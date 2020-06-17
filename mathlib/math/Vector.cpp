// Copyright (C) 2004 Id Software, Inc.
//


#pragma hdrstop
//#include "vector.h"
//#include "../str.h"
#include "../dmathheader.h"
//#ifdef _WIN32
#pragma  warning(push)
#pragma  warning(disable : 4311)
#pragma  warning(disable : 4312)
//#endif
H3DVec2 vec2_origin( 0.0f, 0.0f );
H3DVec3 vec3_origin( 0.0f, 0.0f, 0.0f );
H3DVec3 vec3_unitX( 1.0f, 0.0f, 0.0f );
H3DVec3 vec3_unitY( 0.0f, 1.0f, 0.0f );
H3DVec3 vec3_unitZ( 0.0f, 0.0f, 1.0f );
H3DVec4 vec4_origin( 0.0f, 0.0f, 0.0f, 0.0f );
H3DVec5 vec5_origin( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
H3DVec6 vec6_origin( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
H3DVec6 vec6_infinity( H3DMath::INFINITY, H3DMath::INFINITY, H3DMath::INFINITY, H3DMath::INFINITY, H3DMath::INFINITY, H3DMath::INFINITY );


//===============================================================
//
//	H3DVec2
//
//===============================================================

/*
=============
H3DVec2::ToString
=============
*/
//const char *H3DVec2::ToString( int precision ) const {
//	return H3DStr::FloatArrayToString( ToFloatPtr(), GetDimension(), precision );
//}

/*
=============
Lerp

Linearly inperpolates one vector to another.
=============
*/
void H3DVec2::Lerp( const H3DVec2 &v1, const H3DVec2 &v2, const float l ) {
	if ( l <= 0.0f ) {
		(*this) = v1;
	} else if ( l >= 1.0f ) {
		(*this) = v2;
	} else {
		(*this) = v1 + l * ( v2 - v1 );
	}
}


//===============================================================
//
//	H3DVec3
//
//===============================================================

/*
=============
H3DVec3::ToYaw
=============
*/
float H3DVec3::ToYaw( void ) const {
	float yaw;
	
	if ( ( y == 0.0f ) && ( x == 0.0f ) ) {
		yaw = 0.0f;
	} else {
		yaw = (float)RAD2DEG( atan2( y, x ) );
		if ( yaw < 0.0f ) {
			yaw += 360.0f;
		}
	}

	return yaw;
}

/*
=============
H3DVec3::ToPitch
=============
*/
float H3DVec3::ToPitch( void ) const {
	float	forward;
	float	pitch;
	
	if ( ( x == 0.0f ) && ( y == 0.0f ) ) {
		if ( z > 0.0f ) {
			pitch = 90.0f;
		} else {
			pitch = 270.0f;
		}
	} else {
		forward = ( float )H3DMath::Sqrt( x * x + y * y );
		pitch = (float)RAD2DEG( atan2( z, forward ) );
		if ( pitch < 0.0f ) {
			pitch += 360.0f;
		}
	}

	return pitch;
}

/*
=============
H3DVec3::ToAngles
=============
*/
H3DAngles H3DVec3::ToAngles( void ) const {
	float forward;
	float yaw;
	float pitch;
	
	if ( ( x == 0.0f ) && ( y == 0.0f ) ) {
		yaw = 0.0f;
		if ( z > 0.0f ) {
			pitch = 90.0f;
		} else {
			pitch = 270.0f;
		}
	} else {
		yaw = (float)RAD2DEG( atan2( y, x ) );
		if ( yaw < 0.0f ) {
			yaw += 360.0f;
		}

		forward = ( float )H3DMath::Sqrt( x * x + y * y );
		pitch = (float)RAD2DEG( atan2( z, forward ) );
		if ( pitch < 0.0f ) {
			pitch += 360.0f;
		}
	}

	return H3DAngles( -pitch, yaw, 0.0f );
}

/*
=============
H3DVec3::ToPolar
=============
*/
idPolar3 H3DVec3::ToPolar( void ) const {
	float forward;
	float yaw;
	float pitch;
	
	if ( ( x == 0.0f ) && ( y == 0.0f ) ) {
		yaw = 0.0f;
		if ( z > 0.0f ) {
			pitch = 90.0f;
		} else {
			pitch = 270.0f;
		}
	} else {
		yaw = (float)RAD2DEG( atan2( y, x ) );
		if ( yaw < 0.0f ) {
			yaw += 360.0f;
		}

		forward = ( float )H3DMath::Sqrt( x * x + y * y );
		pitch = (float)RAD2DEG( atan2( z, forward ) );
		if ( pitch < 0.0f ) {
			pitch += 360.0f;
		}
	}
	return idPolar3( H3DMath::Sqrt( x * x + y * y + z * z ), yaw, -pitch );
}

/*
=============
H3DVec3::ToMat3
=============
*/
H3DMat3 H3DVec3::ToMat3( void ) const {
	H3DMat3	mat;
	float	d;

	mat[0] = *this;
	d = x * x + y * y;
	if ( !d ) {
		mat[1][0] = 1.0f;
		mat[1][1] = 0.0f;
		mat[1][2] = 0.0f;
	} else {
		d = H3DMath::InvSqrt( d );
		mat[1][0] = -y * d;
		mat[1][1] = x * d;
		mat[1][2] = 0.0f;
	}
	mat[2] = Cross( mat[1] );

	return mat;
}

/*
=============
H3DVec3::ToString
=============
*/
//const char *H3DVec3::ToString( int precision ) const {
//	return H3DStr::FloatArrayToString( ToFloatPtr(), GetDimension(), precision );
//}

/*
=============
Lerp

Linearly inperpolates one vector to another.
=============
*/
void H3DVec3::Lerp( const H3DVec3 &v1, const H3DVec3 &v2, const float l ) {
	if ( l <= 0.0f ) {
		(*this) = v1;
	} else if ( l >= 1.0f ) {
		(*this) = v2;
	} else {
		(*this) = v1 + l * ( v2 - v1 );
	}
}

/*
=============
SLerp

Spherical linear interpolation from v1 to v2.
Vectors are expected to be normalized.
=============
*/
#define LERP_DELTA 1e-6

void H3DVec3::SLerp( const H3DVec3 &v1, const H3DVec3 &v2, const float t ) {
	float omega, cosom, sinom, scale0, scale1;

	if ( t <= 0.0f ) {
		(*this) = v1;
		return;
	} else if ( t >= 1.0f ) {
		(*this) = v2;
		return;
	}

	//cosom = v1 * v2;
	cosom = v1.Dot(v2);
	if ( ( 1.0f - cosom ) > LERP_DELTA ) {
		omega = (float)acos( cosom );
		sinom = (float)sin( omega );
		scale0 = (float)sin( ( 1.0f - t ) * omega ) / sinom;
		scale1 = (float)sin( t * omega ) / sinom;
	} else {
		scale0 = 1.0f - t;
		scale1 = t;
	}

	(*this) = ( v1 * scale0 + v2 * scale1 );
}

/*
=============
ProjectSelfOntoSphere

Projects the z component onto a sphere.
=============
*/
void H3DVec3::ProjectSelfOntoSphere( const float radius ) {
	float rsqr = radius * radius;
	float len = Length();
	if ( len  < rsqr * 0.5f ) {
		z = (float)sqrt( rsqr - len );
	} else {
		z = (float)(rsqr / ( 2.0f * sqrt( len ) ));
	}
}



//===============================================================
//
//	H3DVec4
//
//===============================================================

/*
=============
H3DVec4::ToString
=============
*/
//const char *H3DVec4::ToString( int precision ) const {
//	return H3DStr::FloatArrayToString( ToFloatPtr(), GetDimension(), precision );
//}

/*
=============
Lerp

Linearly inperpolates one vector to another.
=============
*/
void H3DVec4::Lerp( const H3DVec4 &v1, const H3DVec4 &v2, const float l ) {
	if ( l <= 0.0f ) {
		(*this) = v1;
	} else if ( l >= 1.0f ) {
		(*this) = v2;
	} else {
		(*this) = v1 + l * ( v2 - v1 );
	}
}


//===============================================================
//
//	H3DVec5
//
//===============================================================

/*
=============
H3DVec5::ToString
=============
*/
//const char *H3DVec5::ToString( int precision ) const {
//	return H3DStr::FloatArrayToString( ToFloatPtr(), GetDimension(), precision );
//}

/*
=============
H3DVec5::Lerp
=============
*/
void H3DVec5::Lerp( const H3DVec5 &v1, const H3DVec5 &v2, const float l ) {
	if ( l <= 0.0f ) {
		(*this) = v1;
	} else if ( l >= 1.0f ) {
		(*this) = v2;
	} else {
		x = v1.x + l * ( v2.x - v1.x );
		y = v1.y + l * ( v2.y - v1.y );
		z = v1.z + l * ( v2.z - v1.z );
		s = v1.s + l * ( v2.s - v1.s );
		t = v1.t + l * ( v2.t - v1.t );
	}
}


//===============================================================
//
//	H3DVec6
//
//===============================================================

/*
=============
H3DVec6::ToString
=============
*/
//const char *H3DVec6::ToString( int precision ) const {
//	return H3DStr::FloatArrayToString( ToFloatPtr(), GetDimension(), precision );
//}


//===============================================================
//
//	H3DVecX
//
//===============================================================

float	H3DVecX::temp[VECX_MAX_TEMP+4];
float *	H3DVecX::tempPtr = (float *) ( ( (int) H3DVecX::temp + 15 ) & ~15 );
int		H3DVecX::tempIndex = 0;

/*
=============
H3DVecX::ToString
=============
*/
//const char *H3DVecX::ToString( int precision ) const {
//	return H3DStr::FloatArrayToString( ToFloatPtr(), GetDimension(), precision );
//}

#pragma  warning(pop)
