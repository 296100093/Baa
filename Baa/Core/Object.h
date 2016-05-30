#pragma once

class BbMod
{
public:
	static unsigned long GetModId();
	template<class T>	T* GetMod();

private:
	BbMod* GetMod( unsigned long id );
};

class BbObject : public BbMod
{
public:
	BbObject();

	virtual ~BbObject();
	virtual void OnEvent( BbObject* sender, int evid, void* data );

	void InitObj( BbObject* lis, int id );
	int GetId();

	void PostEvent( int evid, void* data = nullptr );

private:
	BbObject*	m_objListen;
	int			m_objId;
};

template<class T>
T* BbMod::GetMod()
{
	return static_cast<T*>(GetMod(T::GetModId()));
}