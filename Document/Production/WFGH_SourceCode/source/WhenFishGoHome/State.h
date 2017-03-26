#ifndef __STATE_H__
#define __STATE_H__


class State
{
public:
	State(){};// : m_isActive(true){};
	virtual ~State(){};
	virtual bool Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void ResetData() = 0;

	virtual void LMouseDown(int mouse_x, int mouse_y) = 0;
	virtual void LMouseUp(int mouse_x, int mouse_y) = 0;
	virtual void MouseMove(int mouse_x, int mouse_y) = 0;

	void Pause() { m_isActive = false; };
	void Resume() { m_isActive = true; };

protected:
	bool m_isActive;
};

#endif