#pragma once
#include "CustomMenuGlobals.h"

class c_screen_with_menu_vtbl
{
public:
	// virtual class, with default implementation
	virtual int deconstructor(char a2)
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		int result = getVtbl()->deconstructor(a2);

		// restore old vtbl
		*(void**)this = thisVtbl;

		return result;
	};

	virtual int sub_611E23()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		int result = getVtbl()->sub_611E23();
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};

	virtual int sub_611488()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		int result = getVtbl()->sub_611488();
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};
	virtual int sub_612CD8()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		int result = getVtbl()->sub_612CD8();
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};

	virtual void nullsub_213(int a1) {};

	virtual DWORD sub_6114E0(DWORD *a2)
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		DWORD result = getVtbl()->sub_6114E0(a2);
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};

	virtual int sub_611692(int a2)
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		int result = getVtbl()->sub_611692(a2);
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};
	virtual int sub_6116BD()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		int result = getVtbl()->sub_6116BD();
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};

	virtual DWORD* sub_611703(DWORD *a2)
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		DWORD* result = getVtbl()->sub_611703(a2);
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};

	virtual void sub_612A7C()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		getVtbl()->sub_612A7C();
		// restore old vtbl
		*(void**)this = thisVtbl;
	};

	virtual int sub_612ABC()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		int result = getVtbl()->sub_612ABC();
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};

	virtual int sub_612BCA()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		int result = getVtbl()->sub_612BCA();
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};

	virtual int sub_60EEBE(DWORD *a2)
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		int result = getVtbl()->sub_60EEBE(a2);
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};

	virtual int sub_6120F8()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		int result = getVtbl()->sub_6120F8();
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};

	virtual int sub_612120()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		int result = getVtbl()->sub_612120();
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};

	virtual void nullsub_227(int a1) {};
	virtual void nullsub_228(int a1) {};

	virtual int sub_60EF08(int a2)
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		int result = getVtbl()->sub_60EF08(a2);
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};

	virtual char sub_40AD4D()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		char result = getVtbl()->sub_40AD4D();
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};

	virtual int sub_40AD50()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		int result = getVtbl()->sub_40AD50();
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};

	virtual bool sub_60E885()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		bool result = getVtbl()->sub_60E885();
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};

	virtual void nullsub_138() {};

	virtual int sub_6111AB(int a2)
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		int result = getVtbl()->sub_6111AB(a2);
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};

	virtual int sub_60F790()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		int result = getVtbl()->sub_60F790();
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};

	virtual BYTE* sub_610B4E()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		BYTE* result = getVtbl()->sub_610B4E();
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};

	virtual int sub_6102C5()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		int result = getVtbl()->sub_6102C5();
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};
	virtual int sub_6103D6()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		int result = getVtbl()->sub_6103D6();
		// restore old vtbl
		*(void**)this = thisVtbl;

		return result;
	};
	virtual int sub_60F1F4(int a2)
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		int result = getVtbl()->sub_60F1F4(a2);
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};
	virtual char sub_60EFC1(int *a2)
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		char result = getVtbl()->sub_60EFC1(a2);
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};
	virtual char sub_60F081()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		char result = getVtbl()->sub_60F081();
		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};
	virtual signed int sub_60F151(int a2)
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;

		signed int result = getVtbl()->sub_60F151(a2);

		// restore old vtbl
		*(void**)this = thisVtbl;

		return result;
	};
	virtual char sub_40AD53(int a1)
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		char result = getVtbl()->sub_40AD53(a1);

		// restore old vtbl
		*(void**)this = thisVtbl;

		return result;
	};
	virtual int sub_60EB8A()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		int result = getVtbl()->sub_60EB8A();
		// restore old vtbl
		*(void**)this = thisVtbl;

		return result;
	};
	virtual int sub_60EB8E()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		int result = getVtbl()->sub_60EB8E();

		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};
	virtual unsigned int sub_60EB92(int a2)
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;

		unsigned int result = getVtbl()->sub_60EB92(a2);

		// restore old vtbl
		*(void**)this = thisVtbl;

		return result;
	};

	virtual void nullsub_139(int a1) {};

	virtual int sub_60EC5C(DWORD *a2)
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;
		
		int result =getVtbl()->sub_60EC5C(a2);

		// restore old vtbl
		*(void**)this = thisVtbl;

		return result;
	};

	virtual int sub_60ECC9(int a2)
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;

		int result = getVtbl()->sub_60ECC9(a2);

		// restore old vtbl
		*(void**)this = thisVtbl;

		return result;
	};
	virtual void sub_688255()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;

		getVtbl()->sub_688255();

		// restore old vtbl
		*(void**)this = thisVtbl;
	};

	virtual char sub_40AD58()
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;

		char result = getVtbl()->sub_40AD58();

		// restore old vtbl
		*(void**)this = thisVtbl;
		return result;
	};

	virtual void sub_60F2A4(char a2)
	{
		// store the old vtable
		void* thisVtbl = *(void**)this;

		getVtbl()->sub_60F2A4(a2);

		// restore old vtbl
		*(void**)this = thisVtbl;
	};

	virtual c_screen_with_menu_vtbl* getVtbl();
};

class c_screen_with_menu : public c_screen_with_menu_vtbl
{
public:
	c_screen_with_menu::c_screen_with_menu(int menu_wgit_type, int a3, int a4, __int16 a5, void* a6);

	char data[4096];

};
//static_assert(sizeof(c_screen_with_menu) == 4004, "Warning size of c_screen_with_menu != 0xA4");