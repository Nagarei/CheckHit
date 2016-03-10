#ifndef CHECKHIT_H_20150619164015611654
#define CHECKHIT_H_20150619164015611654
#include <memory>
#include <utility>
#include <functional>
#include "Square.hpp"

#include "CheckHit_ISCONTACTconcept.h"
#ifdef _MSC_VER
#pragma warning( disable : 4512 )//代入演算子を生成できません。
#pragma warning( disable : 4505 )//参照されていないローカル関数は削除されました。
#endif

namespace CheckHit
{

	template<typename T>
	class Object_Bace
	{
	public:
		typedef T value_type;
		enum Type : int{ Dot, Line, Triangle, AABB, Circle, Polygon, Type_END };
		virtual Type GetType()const = 0;
		virtual bool IsContact(const Object_Bace<T>& obj2)const = 0;
		virtual ~Object_Bace(){}
	};


	//----------IsContact----------//

	template<typename ObjectT, typename T>
	typename std::enable_if<std::is_base_of<Object_Bace<T>, ObjectT>::value, bool>::type
		IsContact(const ObjectT& obj1, const Object_Bace<T>& obj2);
	template<typename T, typename ObjectT>
	typename std::enable_if<std::is_base_of<Object_Bace<T>, ObjectT>::value, bool>::type
		IsContact(const Object_Bace<T>& obj1, const ObjectT& obj2);
	ISCONTACT_concept IsContact(const Object_Bace<T1>& obj1, const Object_Bace<T2>& obj2);

	namespace CheckHitImpl
	{
		template<typename T, typename Child>
		class Object_Impl_Bace : public Object_Bace < T >
		{
		public:
			bool IsContact(const Object_Bace<T>& obj2)const override;
		};
	}


	//----------Object----------//

	namespace CheckHitImpl
	{
		template <typename T_b, typename T = typename remove_crv_wrapper<T_b>::type>
		struct add_clr{ typedef std::reference_wrapper<typename std::add_const<typename std::remove_reference<T>::type>::type> type; };
		template <typename T_b, typename T = typename remove_crv_wrapper<T_b>::type>
		struct add_cr{ typedef typename std::add_const < typename std::remove_reference<T>::type > ::type type; };
	}

	template<typename T>
	class Object_Dot : public CheckHitImpl::Object_Impl_Bace < T, Object_Dot<T> >
	{
	public:
		typedef const T& cr_T;
		T x, y;
		Object_Dot(){}
		template<typename U, typename = typename std::enable_if<std::is_constructible<T,U>::value>::type>
		Object_Dot(const Object_Dot<U>& d);
		Object_Dot(cr_T param_x, cr_T param_y);

		OBJECT_operator_equal_with_concept(Object_Dot);

		//operator Object_Dot<cr_T>()const{ return Object_Dot<cr_T>(*this); }
		Type GetType()const override{ return Dot; }
	};
	template < typename T >
	class Object_Line : public CheckHitImpl::Object_Impl_Bace < T, Object_Line<T> >
	{
	public:
		typedef const T& cr_T;
		T x1, y1;
		T x2, y2;
		Object_Line(){}
		template<typename U, typename = typename std::enable_if<std::is_constructible<T, U>::value>::type>
		Object_Line(const Object_Line<U>& l);
		template<typename U, typename = typename std::enable_if<std::is_constructible<T, U>::value>::type>
		Object_Line(const Object_Dot<U>& d1, const Object_Dot<U>& d2);
		Object_Line(cr_T param_x1, cr_T param_y1
			, cr_T param_x2, cr_T param_y2);

		OBJECT_operator_equal_with_concept(Object_Line);

		//operator Object_Line<cr_T>()const{ return Object_Line<cr_T>(*this); }
		Type GetType()const override{ return Line; }
	};
	template < typename T >
	class Object_Triangle : public CheckHitImpl::Object_Impl_Bace < T, Object_Triangle<T> >
	{
	public:
		typedef const T& cr_T;
		T x1, y1;
		T x2, y2;
		T x3, y3;
		Object_Triangle(){}
		template<typename U, typename = typename std::enable_if<std::is_constructible<T, U>::value>::type>
		Object_Triangle(const Object_Triangle<U>& tri);
		template<typename U, typename = typename std::enable_if<std::is_constructible<T, U>::value>::type>
		Object_Triangle(const Object_Dot<U>& d1, const Object_Dot<U>& d2, const Object_Dot<U>& d3);
		Object_Triangle(cr_T param_x1, cr_T param_y1
			, cr_T param_x2, cr_T param_y2
			, cr_T param_x3, cr_T param_y3);

		OBJECT_operator_equal_with_concept(Object_Triangle);

		//operator Object_Triangle<cr_T>()const{ return Object_Triangle<cr_T>(*this); }
		Type GetType()const override{ return Triangle; }
	};
	template<typename T>
	//回転してない長方形
	class Object_AABB : public CheckHitImpl::Object_Impl_Bace < T, Object_AABB<T> >
	{
	public:
		typedef const T& cr_T;
		T x1, y1;
		T x2, y2;

		Object_AABB(){}
		template<typename U, typename = typename std::enable_if<std::is_constructible<T, U>::value>::type>
		Object_AABB(const Object_AABB<U>& AABB);
		template<typename U, typename = typename std::enable_if<std::is_constructible<T, U>::value>::type>
		Object_AABB(const Object_Dot<U>& d1, const Object_Dot<U>& d2);
		Object_AABB(cr_T param_x1, cr_T param_y1, cr_T param_x2, cr_T param_y2);

		OBJECT_operator_equal_with_concept(Object_AABB);

		//Square_Pointとの互換

		template<typename U, typename = typename std::enable_if<std::is_constructible<T, U>::value>::type>
		Object_AABB(const Square_Point<U>& s);
		template<typename U, typename = typename std::enable_if<std::is_constructible<T, U>::value>::type>
		Object_AABB<T>& operator=(const Square_Point<U>& s);
		operator Square_Point<T>();

		//operator Object_AABB<cr_T>()const{ return Object_AABB<cr_T>(*this); }
		Type GetType()const override{ return AABB; }
	};
	template<typename T>
	class Object_Circle : public CheckHitImpl::Object_Impl_Bace < T, Object_Circle<T> >
	{
	public:
		typedef const T& cr_T;
		T x, y, r;
		Object_Circle(){}
		template<typename U, typename = typename std::enable_if<std::is_constructible<T, U>::value>::type>
		Object_Circle(const Object_Circle<U>& cir);
		template<typename U, typename = typename std::enable_if<std::is_constructible<T, U>::value>::type>
		Object_Circle(const Object_Dot<U>& center, cr_T param_r);
		Object_Circle(cr_T param_x, cr_T param_y, cr_T param_r);

		OBJECT_operator_equal_with_concept(Object_Circle);

		//operator Object_Circle<cr_T>()const{ return Object_Circle<cr_T>(*this); }
		Type GetType()const override{ return Circle; }
	};

	//Container : std::vector<Object_Bace<T>*>など
	//配列は使えない
	template<typename T, typename Container>
	//複数の図形を組み合わせて一つの図形として扱う
	class Object_Combined : public CheckHitImpl::Object_Impl_Bace < T, Object_Combined<T, Container> >
	{
	public:
		typedef Object_Combined<T, Container> This_T;

		Container objects;

		Object_Combined(){}
		Object_Combined(const This_T& polygon);
		Object_Combined(This_T&& polygon);
		Object_Combined(const Container& objects);
		Object_Combined(Container&& objects);

		Object_Combined<T, Container>& operator=(const This_T& polygon);
		Object_Combined<T, Container>& operator=(This_T&& polygon);
		Object_Combined<T, Container>& operator=(const Container& objects);
		Object_Combined<T, Container>& operator=(Container&& objects);

		Type GetType()const override{ return Polygon; }
	};
}
#include "CheckHit_ISCONTACTconcept_end.h"

#include "CheckHit.hpp"
#endif
