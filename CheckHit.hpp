#ifndef CHECK_HIT_HPP_2201506192034485961841562
#define CHECK_HIT_HPP_2201506192034485961841562
#include "CheckHit.h"
#include "CheckHitImpl.h"
#include <algorithm>
#include <cassert>

#include "CheckHit_ISCONTACTconcept.h"

namespace CheckHit
{

	//----------IsContact----------//

	template<typename ObjectT, typename T>
	inline typename std::enable_if<std::is_base_of<Object_Bace<T>, ObjectT>::value, bool>::type
		IsContact(const ObjectT& obj1, const Object_Bace<T>& obj2)
	{
		static_assert(Object_Bace<T>::Type::Type_END == 6, "");
		switch (obj2.GetType())
		{
		case Object_Bace<T>::Dot:
			return CheckHit::IsContact(obj1, static_cast<const Object_Dot<T>&>(obj2));
		case Object_Bace<T>::Line:
			return CheckHit::IsContact(obj1, static_cast<const Object_Line<T>&>(obj2));
		case Object_Bace<T>::Triangle:
			return CheckHit::IsContact(obj1, static_cast<const Object_Triangle<T>&>(obj2));
		case Object_Bace<T>::AABB:
			return CheckHit::IsContact(obj1, static_cast<const Object_AABB<T>&>(obj2));
		case Object_Bace<T>::Circle:
			return CheckHit::IsContact(obj1, static_cast<const Object_Circle<T>&>(obj2));
		case Object_Bace<T>::Polygon:
			//特殊ケース
			//相手がObject_Combinedの場合は相手に任せる
			return obj2.IsContact(obj1);
		default:
			assert(false);
			return false;
		}
	}
	template<typename T, typename ObjectT>
	inline typename std::enable_if<std::is_base_of<Object_Bace<T>, ObjectT>::value, bool>::type
		IsContact(const Object_Bace<T>& obj1, const ObjectT& obj2)
	{
		return IsContact(obj2, obj1);
	}
	ISCONTACT_concept_inline IsContact(const Object_Bace<T1>& obj1, const Object_Bace<T2>& obj2)
	{
		//反転させて呼び出すことで組み合わせを変化させる
		//（(A,B)のオーバーロードはあるが、(B,A)のオーバーロードがない状況でも動くように）
		return obj2.IsContact(obj1);
	}
	template<typename T, typename Child>
	inline bool CheckHitImpl::Object_Impl_Bace<T, Child>::IsContact(const Object_Bace<T>& obj2)const
	{
		return CheckHit::IsContact(static_cast<const Child&>(*this), obj2);
	}


	//----------Object----------//

	//-----Dot

	template<typename T>
	template<typename U, typename>
	inline Object_Dot<T>::Object_Dot(const Object_Dot<U>& d)
		: x(d.x)
		, y(d.y)
	{}
	template<typename T>
	inline Object_Dot<T>::Object_Dot(cr_T param_x, cr_T param_y)
		: x(param_x)
		, y(param_y)
	{}
	OBJECT_operator_equal_with_concept_define(Object_Dot)
	{
		x = obj2.x;
		y = obj2.y;
	}


	//-----Line

	template<typename T>
	template<typename U, typename>
	inline Object_Line<T>::Object_Line(const Object_Line<U>& l)
		: x1(l.x1), y1(l.y1)
		, x2(l.x2), y2(l.y2)
	{}
	template<typename T>
	inline Object_Line<T>::Object_Line
		(cr_T param_x1, cr_T param_y1
		, cr_T param_x2, cr_T param_y2)
		: x1(param_x1), y1(param_y1)
		, x2(param_x2), y2(param_y2)
	{}
	template<typename T>
	template<typename U, typename>
	inline Object_Line<T>::Object_Line(const Object_Dot<U>& d1, const Object_Dot<U>& d2)
		: x1(d1.x), y1(d1.y)
		, x2(d2.x), y2(d2.y)
	{}
	OBJECT_operator_equal_with_concept_define(Object_Line)
	{
		x1 = obj2.x1; y1 = obj2.y1;
		x2 = obj2.x2; y2 = obj2.y2;
	}


	//-----Triangle

	template<typename T>
	template<typename U, typename>
	inline Object_Triangle<T>::Object_Triangle(const Object_Triangle<U>& tri)
		: x1(tri.x1), y1(tri.y1)
		, x2(tri.x2), y2(tri.y2)
		, x3(tri.x3), y3(tri.y3)
	{}
	template<typename T>
	template<typename U, typename>
	inline Object_Triangle<T>::Object_Triangle(const Object_Dot<U>& d1, const Object_Dot<U>& d2, const Object_Dot<U>& d3)
		: x1(d1.x), y1(d1.y)
		, x2(d2.x), y2(d2.y)
		, x3(d3.x), y3(d3.y)
	{}
	template<typename T>
	inline Object_Triangle<T>::Object_Triangle
		(cr_T param_x1, cr_T param_y1
		, cr_T param_x2, cr_T param_y2
		, cr_T param_x3, cr_T param_y3)
		: x1(param_x1), y1(param_y1)
		, x2(param_x2), y2(param_y2)
		, x3(param_x3), y3(param_y3)
	{}
	OBJECT_operator_equal_with_concept_define(Object_Triangle)
	{
		x1 = obj2.x1; y1 = obj2.y1;
		x2 = obj2.x2; y2 = obj2.y2;
		x3 = obj2.x3; y3 = obj2.y3;
	}


	//-----AABB

	template<typename T>
	template<typename U, typename>
	inline Object_AABB<T>::Object_AABB(const Object_AABB<U>& AABB)
		: x1(AABB.x1), y1(AABB.y1)
		, x2(AABB.x2), y2(AABB.y2)
	{}
	template<typename T>
	template<typename U, typename>
	inline Object_AABB<T>::Object_AABB(const Object_Dot<U>& d1, const Object_Dot<U>& d2)
		: x1(d1.x), y1(d1.y)
		, x2(d2.x), y2(d2.y)
	{}
	template<typename T>
	inline Object_AABB<T>::Object_AABB(cr_T param_x1, cr_T param_y1, cr_T param_x2, cr_T param_y2)
		: x1(param_x1)
		, y1(param_y1)
		, x2(param_x2)
		, y2(param_y2)
	{}
	OBJECT_operator_equal_with_concept_define(Object_AABB)
	{
		x1 = obj2.x1; y1 = obj2.y1;
		x2 = obj2.x2; y2 = obj2.y2;
	}
	template<typename T>
	template<typename U, typename>
	inline Object_AABB<T>::Object_AABB(const Square_Point<U>& s)
		: x1(s.Left_Side)
		, y1(s.Top_Side)
		, x2(s.Right_Side)
		, y2(s.Bottom_Side)
	{}
	template<typename T>
	template<typename U, typename>
	inline Object_AABB<T>& Object_AABB<T>::operator=(const Square_Point<U>& s)
	{
		x1 = s.Left_Side;
		y1 = s.Top_Side;
		x2 = s.Right_Side;
		y2 = s.Bottom_Side;
		return *this;
	}
	template<typename T>
	inline Object_AABB<T>::operator Square_Point<T>()
	{
		return Square_Point<T>(y1, y2, x2, x1);
	}


	//-----Circle

	template<typename T>
	template<typename U, typename>
	inline Object_Circle<T>::Object_Circle(const Object_Circle<U>& cir)
		: x(cir.x)
		, y(cir.y)
		, r(cir.r)
	{}
	template<typename T>
	inline Object_Circle<T>::Object_Circle(cr_T param_x, cr_T param_y, cr_T param_r)
		: x(param_x)
		, y(param_y)
		, r(param_r)
	{}
	template<typename T>
	template<typename U, typename>
	inline Object_Circle<T>::Object_Circle(const Object_Dot<U>& center, cr_T param_r)
		: x(center.x)
		, y(center.y)
		, r(param_r)
	{}
	OBJECT_operator_equal_with_concept_define(Object_Circle)
	{
		x = obj2.x;
		y = obj2.y;
		r = obj2.r;
	}


	//-----Polygon

	template<typename T, typename Container>
	inline Object_Combined<T, Container>::Object_Combined(const This_T& polygon)
		: objects(polygon.objects)
	{}
	template<typename T, typename Container>
	inline Object_Combined<T, Container>::Object_Combined(This_T&& polygon)
		: objects(std::move(polygon.objects))
	{}
	template<typename T, typename Container>
	inline 	Object_Combined<T, Container>::Object_Combined(const Container& param_objects)
		: objects(param_objects)
	{}
	template<typename T, typename Container>
	inline Object_Combined<T, Container>::Object_Combined(Container&& param_objects)
		: objects(std::move(param_objects))
	{}
	template<typename T, typename Container>
	inline Object_Combined<T, Container>& Object_Combined<T, Container>::operator=(const This_T& polygon)
	{
		objects = polygon.objects;
	}
	template<typename T, typename Container>
	inline Object_Combined<T, Container>& Object_Combined<T, Container>::operator=(This_T&& polygon)
	{
		objects = std::move(polygon.objects);
	}
	template<typename T, typename Container>
	inline Object_Combined<T, Container>& Object_Combined<T, Container>::operator=(const Container& param_objects)
	{
		objects = param_objects;
	}
	template<typename T, typename Container>
	inline Object_Combined<T, Container>& Object_Combined<T, Container>::operator=(Container&& param_objects)
	{
		objects = std::move(param_objects);
	}


}
#include "CheckHit_ISCONTACTconcept_end.h"

#endif
