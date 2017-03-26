#include "stdafx.h"
#include "Camera.h"

Camera* Camera::m_instance = NULL;

Camera::Camera()
{}

Camera::~Camera()
{
	m_instance = NULL;
}

Camera* Camera::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new Camera();
	}

	return m_instance;
}

void Camera::SetLookAt()
{
	Vector3 f = (m_vLook - m_vEye).Normalize();
	Vector3 s = f.Cross(m_vUp).Normalize();
	Vector3 u = s.Cross(f).Normalize();

	m_CameraView.m[0][0] = s.x;
	m_CameraView.m[1][0] = s.y;
	m_CameraView.m[2][0] = s.z;
	m_CameraView.m[3][0] = -s.Dot(m_vEye);

	m_CameraView.m[0][1] = u.x;
	m_CameraView.m[1][1] = u.y;
	m_CameraView.m[2][1] = u.z;
	m_CameraView.m[3][1] = -u.Dot(m_vEye);

	m_CameraView.m[0][2] = -f.x;
	m_CameraView.m[1][2] = -f.y;
	m_CameraView.m[2][2] = -f.z;
	m_CameraView.m[3][2] = f.Dot(m_vEye);

	m_CameraView.m[0][3] = 0.0f;
	m_CameraView.m[1][3] = 0.0f;
	m_CameraView.m[2][3] = 0.0f;
	m_CameraView.m[3][3] = 1.0f;
}

void Camera::SetFBOLookAt()
{
	m_fboEye.x = m_vEye.x;
	m_fboEye.z = m_vEye.z;

	Vector3 f = (m_vLook - m_fboEye).Normalize();
	Vector3 s = f.Cross(m_vUp).Normalize();
	Vector3 u = s.Cross(f).Normalize();

	m_CameraView.m[0][0] = s.x;
	m_CameraView.m[1][0] = s.y;
	m_CameraView.m[2][0] = s.z;
	m_CameraView.m[3][0] = -s.Dot(m_fboEye);

	m_CameraView.m[0][1] = u.x;
	m_CameraView.m[1][1] = u.y;
	m_CameraView.m[2][1] = u.z;
	m_CameraView.m[3][1] = -u.Dot(m_fboEye);

	m_CameraView.m[0][2] = -f.x;
	m_CameraView.m[1][2] = -f.y;
	m_CameraView.m[2][2] = -f.z;
	m_CameraView.m[3][2] = f.Dot(m_fboEye);

	m_CameraView.m[0][3] = 0.0f;
	m_CameraView.m[1][3] = 0.0f;
	m_CameraView.m[2][3] = 0.0f;
	m_CameraView.m[3][3] = 1.0f;
}