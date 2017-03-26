#ifndef __CAMERA_H__
#define __CAMERA_H__

class Camera
{
public:
	static Camera* getInstance();
	~Camera();
	void SetLookAt();
	void SetFBOLookAt();

	Matrix m_CameraView;
	Matrix m_Projection;

	Vector3 m_vEye;
	Vector3 m_vLook;
	Vector3 m_vUp;

	Vector3 m_fboEye;
private:
	Camera();
	static Camera *m_instance;
};

#endif