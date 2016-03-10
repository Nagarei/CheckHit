//*佐々木君のコードの改変
#ifndef CHECK_HIT_IMPL_HPP_20150619210485684856156
#define CHECK_HIT_IMPL_HPP_20150619210485684856156
#include "CheckHitImpl.h"

#include <cstdint>

#include "CheckHit_ISCONTACTconcept.h"
namespace CheckHit
{

	namespace CheckHitImpl
	{
		template<typename T1, typename T2>
		//2D有効線分の外積を取る
		inline auto Cross(const Object_Line<T1>& Line1, const Object_Line<T2>& Line2)
			->decltype((Line1.x2 - Line1.x1)*(Line2.y2 - Line2.y1) - (Line2.x2 - Line2.x1)*(Line1.y2 - Line1.y1))
		{
			return (Line1.x2 - Line1.x1)*(Line2.y2 - Line2.y1) - (Line2.x2 - Line2.x1)*(Line1.y2 - Line1.y1);
		}
		template<typename T1, typename T2>
		//2D有効線分の内積を取る
		inline auto Dot(const Object_Line<T1>& Line1, const Object_Line<T2>& Line2)
			->decltype((Line1.x2 - Line1.x1)*(Line2.x2 - Line2.x1) + (Line1.y2 - Line1.y1)*(Line2.y2 - Line2.y1))
		{
			return (Line1.x2 - Line1.x1)*(Line2.x2 - Line2.x1) + (Line1.y2 - Line1.y1)*(Line2.y2 - Line2.y1);
		}

		template<typename T>
		//点が有向線分の左なら1、右なら-1、線上なら0
		inline int_fast8_t Side(const Object_Line<T>& Line, const Object_Dot<T>& Dot)
		{
			typedef Object_Line<typename CheckHitImpl::add_cr<T>::type> cr_Obj_Line;
			auto c = Cross(Line, cr_Obj_Line(Line.x1, Line.y1, Dot.x, Dot.y));
			//auto c = Dot.x * (Line.y1 - Line.y2) + Line.x1 * (Line.y2 - Dot.y) + Line.x2 * (Dot.y - Line.y1);
			if (c > 0) return 1;
			if (c < 0) return -1;
			return 0;
		}

		namespace HELPER{
			using std::pow;
			template<typename T1, typename T2>
			//2点間の距離の二乗
			inline auto Dist2(const Object_Dot<T1>& Dot1, const Object_Dot<T2>& Dot2)
				->decltype(pow(Dot2.x - Dot1.x, 2) + pow(Dot2.y - Dot1.y, 2))
			{
				return (pow(Dot2.x - Dot1.x, 2) + pow(Dot2.y - Dot1.y, 2));
			}
		}
		using HELPER::Dist2;

		namespace HELPER{
				using std::abs;
			template<typename T>
			//AreaOfTriの2倍
			inline auto AreaOfTri2(const Object_Triangle<T>& Tri)
				->decltype(abs((Tri.x2 - Tri.x1) * (Tri.y3 - Tri.y1) - (Tri.x3 - Tri.x1) * (Tri.y2 - Tri.y1)))
			{
				return abs((Tri.x2 - Tri.x1) * (Tri.y3 - Tri.y1) - (Tri.x3 - Tri.x1) * (Tri.y2 - Tri.y1));// / 2;
			}
		}
		using HELPER::AreaOfTri2;

		template<typename T1, typename T2>
		//value1*value2 <= 0と等価(value1*value2 < 0ではない)
		inline bool IsDifferentSigns(const T1& value1, const T2& value2)
		{
			if (value1 < 0){
				if (value2 >= 0){ return true; }
				else { return false; }
			}
			else{
				if (value2 <= 0){ return true; }
				else if (value1 == 0){ return true; }
				else{ return false; }
			}
		}
	}

	//----------Dot to X ----------//

	ISCONTACT_concept_inline IsContact(const Object_Dot<T1>& Dot1, const Object_Dot<T2>& Dot2)
	{
		return ((Dot1.x == Dot2.x) && (Dot1.y == Dot2.y));
	}
	ISCONTACT_concept_inline IsContact(const Object_Dot<T1>& Dot, const Object_Line<T2>& Line)
	{
		return IsContact(Line, Dot);
	}
	ISCONTACT_concept_inline IsContact(const Object_Dot<T1>& Dot, const Object_Triangle<T2>& Tri)
	{
		return IsContact(Tri, Dot);
	}
	ISCONTACT_concept_inline IsContact(const Object_Dot<T1>& Dot, const Object_AABB<T2>& AABB)
	{
		return ((AABB.x1 <= Dot.x)
			&& (AABB.x2 >= Dot.x)
			&& (AABB.y1 <= Dot.y)
			&& (AABB.y2 >= Dot.y)
			);
	}
	ISCONTACT_concept_inline IsContact(const Object_Dot<T1>& Dot, const Object_Circle<T2>& Cir)
	{
		return IsContact(Cir, Dot);
	}


	//----------Line to X ----------//

	ISCONTACT_concept_inline IsContact(const Object_Line<T1>& Line, const Object_Dot<T2>& Dot)
	{
		typedef typename CheckHitImpl::add_cr<T1>::type cr_T;
		typedef Object_Dot<cr_T> cr_Obj_Dot;
		typedef Object_AABB<cr_T> cr_Obj_AABB;

		cr_Obj_Dot Line_begin(Line.x1, Line.y1);
		cr_Obj_Dot Line_end(Line.x2, Line.y2);

		return (CheckHitImpl::Side(Line, Dot) == 0)
			&& IsContact(cr_Obj_AABB(Line_begin, Line_end), Dot);
	}
	ISCONTACT_concept_inline IsContact(const Object_Line<T1>& Line1, const Object_Line<T2>& Line2)
	{
		using CheckHitImpl::Side;
		typedef typename CheckHitImpl::add_cr<T1>::type cr_T;
		typedef Object_Dot<cr_T> cr_Obj_Dot;
		typedef Object_Line<cr_T> cr_Obj_Line;
		return((Side(cr_Obj_Line(Line1), cr_Obj_Dot(Line2.x1, Line2.y1)) != Side(cr_Obj_Line(Line1), cr_Obj_Dot(Line2.x2, Line2.y2)))
			&& (Side(cr_Obj_Line(Line2), cr_Obj_Dot(Line1.x1, Line1.y1)) != Side(cr_Obj_Line(Line2), cr_Obj_Dot(Line1.x2, Line1.y2)))
			);
	}
	ISCONTACT_concept_inline IsContact(const Object_Line<T1>& Line, const Object_Triangle<T2>& Tri)
	{
		return IsContact(Tri, Line);
	}
	ISCONTACT_concept IsContact(const Object_Line<T1>& Line, const Object_AABB<T2>& AABB)
	{
		using CheckHitImpl::add_cr;

		typedef Object_Line<typename add_cr<T2>::type> cr_Obj_Line;
		if (IsContact(Line, cr_Obj_Line(AABB.x1, AABB.y1, AABB.x1, AABB.y2))){
			return true;//左の辺に接触
		}
		if (IsContact(Line, cr_Obj_Line(AABB.x1, AABB.y2, AABB.x2, AABB.y2))){
			return true;//下の辺に接触
		}
		if (IsContact(Line, cr_Obj_Line(AABB.x2, AABB.y2, AABB.x2, AABB.y1))){
			return true;//右の辺に接触
		}
		if (IsContact(Line, cr_Obj_Line(AABB.x2, AABB.y1, AABB.x1, AABB.y1))){
			return true;//上の辺に接触
		}

		typedef Object_Dot<typename add_cr<T2>::type> cr_Obj_Dot;
		if (IsContact(cr_Obj_Dot(Line.x1, Line.y1), AABB)){
			return true;//始点がめり込んでいる
		}
		if (IsContact(cr_Obj_Dot(Line.x2, Line.y2), AABB)){
			return true;//終点がめり込んでいる
		}

		return false;
	}
	ISCONTACT_concept IsContact(const Object_Line<T1>& Line, const Object_Circle<T2>& Cir)
	{
		using std::pow;
		using std::abs;
		using CheckHitImpl::Dist2;
		using CheckHitImpl::AreaOfTri2;
		using CheckHitImpl::Cross;
		using CheckHitImpl::Dot;
		using CheckHitImpl::IsDifferentSigns;
		typedef typename CheckHitImpl::add_cr<T1>::type cr_T;
		typedef Object_Dot<cr_T> cr_Obj_Dot;
		typedef Object_Line<cr_T> cr_Obj_Line;
		typedef Object_Triangle<cr_T> cr_Obj_Tri;

		cr_Obj_Dot Line_begin(Line.x1, Line.y1);
		cr_Obj_Dot Line_end(Line.x2, Line.y2);
		cr_Obj_Dot R_center(Cir.x, Cir.y);

		cr_Obj_Line Line_BR(Line_begin, R_center);
		cr_Obj_Line Line_ER(Line_end, R_center);

		auto R2 = Cir.r*Cir.r;

#if 1//オーバーフローしないように工夫する
		auto Cross_L_BR = abs((Line.x2 - Line.x1)*(Line_BR.y2 - Line_BR.y1) - (Line_BR.x2 - Line_BR.x1)*(Line.y2 - Line.y1));
		Cross_L_BR = abs(Cross(Line, Line_BR));
		//直線Line、円Cirの中心間の距離が半径以下
		if (R2 / Cross_L_BR >=
			(Cross_L_BR / Dist2(Line_begin, Line_end))
			)
		{
			//直線Lineとはあたっている

			if (IsDifferentSigns(Dot(Line_BR, Line), Dot(Line_ER, Line))){
				return true;//線分Lineの始点と終点が円Cirを挟んでいる
			}
			else if (Dist2(Line_begin, R_center) <= R2 || Dist2(Line_end, R_center) <= R2){
				return true;
			}
		}
		return false;
#else//工夫前のコード
		if (R2 >=//半径以下
			pow(Cross(Line, Line_BR), 2) / Dist2(Line_begin, Line_end)//直線Line、円Cirの中心間の距離が
			)
		{
			//直線Lineとはあたっている

			if (Dot(Line_BR, Line) * Dot(Line_ER, Line) <= 0){
				return true;//線分Lineの始点と終点が円Cirを挟んでいる
			}
			else if (Dist2(Line_begin, R_center) <= R2 || Dist2(Line_end, R_center) <= R2){
				return true;
			}
		}
		return false;
#endif
	}


	//----------Triangle to X ----------//

	ISCONTACT_concept_inline IsContact(const Object_Triangle<T1>& Tri, const Object_Dot<T2>& Dot)
	{
		using CheckHitImpl::Side;
		typedef typename CheckHitImpl::add_cr<T1>::type cr_T;
		typedef Object_Dot<cr_T> cr_Obj_Dot;
		typedef Object_Line<cr_T> cr_Obj_Line;
		cr_Obj_Dot Tri_D1(Tri.x1, Tri.y1);
		cr_Obj_Dot Tri_D2(Tri.x2, Tri.y2);
		cr_Obj_Dot Tri_D3(Tri.x3, Tri.y3);
		cr_Obj_Line Tri_L1(Tri_D1, Tri_D2);
		cr_Obj_Line Tri_L2(Tri_D2, Tri_D3);
		cr_Obj_Line Tri_L3(Tri_D3, Tri_D1);

		auto c1 = Side<cr_T>(Tri_L1, cr_Obj_Dot(Dot));
		auto c2 = Side<cr_T>(Tri_L2, cr_Obj_Dot(Dot));
		auto c3 = Side<cr_T>(Tri_L3, cr_Obj_Dot(Dot));
		if ((c1 >= 0 && c2 >= 0 && c3 >= 0) || (c1 <= 0 && c2 <= 0 && c3 <= 0)){
			return true;
		}
		return false;
	}
	ISCONTACT_concept IsContact(const Object_Triangle<T1>& Tri, const Object_Line<T2>& Line)
	{
		using CheckHitImpl::add_cr;
		typedef Object_Dot<typename add_cr<T1>::type> cr_Obj_Dot;
		typedef Object_Line<typename add_cr<T1>::type> cr_Obj_Line;

		cr_Obj_Dot Line_begin(Line.x1, Line.y1);
		cr_Obj_Dot Line_end(Line.x2, Line.y2);

		if (IsContact(Tri, Line_begin)){ return true; }
		if (IsContact(Tri, Line_end))  { return true; }

		if (IsContact(cr_Obj_Line(Tri.x1, Tri.y1, Tri.x2, Tri.y2), Line)){ return true; }
		if (IsContact(cr_Obj_Line(Tri.x2, Tri.y2, Tri.x3, Tri.y3), Line)){ return true; }
		if (IsContact(cr_Obj_Line(Tri.x3, Tri.y3, Tri.x1, Tri.y1), Line)){ return true; }
		return false;
	}
	ISCONTACT_concept IsContact(const Object_Triangle<T1>& Tri1, const Object_Triangle<T2>& Tri2)
	{
		using CheckHitImpl::add_cr;
		typedef Object_Dot<typename add_cr<T1>::type> cr_Obj_Dot;
		typedef Object_Line<typename add_cr<T1>::type> cr_Obj_Line;

		cr_Obj_Dot Tri1_D1(Tri1.x1, Tri1.y1);
		cr_Obj_Dot Tri1_D2(Tri1.x2, Tri1.y2);
		cr_Obj_Dot Tri1_D3(Tri1.x3, Tri1.y3);
		cr_Obj_Dot Tri2_D1(Tri2.x1, Tri2.y1);
		cr_Obj_Dot Tri2_D2(Tri2.x2, Tri2.y2);
		cr_Obj_Dot Tri2_D3(Tri2.x3, Tri2.y3);

		cr_Obj_Line Tri1_L1(Tri1_D1, Tri1_D2);
		cr_Obj_Line Tri1_L2(Tri1_D2, Tri1_D3);
		cr_Obj_Line Tri1_L3(Tri1_D3, Tri1_D1);
		cr_Obj_Line Tri2_L1(Tri2_D1, Tri2_D2);
		cr_Obj_Line Tri2_L2(Tri2_D2, Tri2_D3);
		cr_Obj_Line Tri2_L3(Tri2_D3, Tri2_D1);

		if (IsContact(Tri1, Tri2_D1)){ return true; }
		if (IsContact(Tri1, Tri2_D2)){ return true; }
		if (IsContact(Tri1, Tri2_D3)){ return true; }
		if (IsContact(Tri2, Tri1_D1)){ return true; }
		if (IsContact(Tri2, Tri1_D2)){ return true; }
		if (IsContact(Tri2, Tri1_D3)){ return true; }

		if (IsContact(Tri1_L1, Tri2_L1)){ return true; }
		if (IsContact(Tri1_L1, Tri2_L2)){ return true; }
		if (IsContact(Tri1_L1, Tri2_L3)){ return true; }
		if (IsContact(Tri1_L2, Tri2_L1)){ return true; }
		if (IsContact(Tri1_L2, Tri2_L2)){ return true; }
		if (IsContact(Tri1_L2, Tri2_L3)){ return true; }
		if (IsContact(Tri1_L3, Tri2_L1)){ return true; }
		if (IsContact(Tri1_L3, Tri2_L2)){ return true; }
		if (IsContact(Tri1_L3, Tri2_L3)){ return true; }

		return false;
	}
	ISCONTACT_concept IsContact(const Object_Triangle<T1>& Tri, const Object_AABB<T2>& AABB)
	{
		using CheckHitImpl::add_cr;
		typedef Object_Dot<typename add_cr<T1>::type> cr_Obj_Dot;
		typedef Object_Line<typename add_cr<T1>::type> cr_Obj_Line;

		cr_Obj_Dot Tri_D1(Tri.x1, Tri.y1);
		cr_Obj_Dot Tri_D2(Tri.x2, Tri.y2);
		cr_Obj_Dot Tri_D3(Tri.x3, Tri.y3);
		cr_Obj_Line Tri_L1(Tri_D1, Tri_D2);
		cr_Obj_Line Tri_L2(Tri_D2, Tri_D3);
		cr_Obj_Line Tri_L3(Tri_D3, Tri_D1);

		cr_Obj_Dot AABB_D1(AABB.x1, AABB.y1);
		cr_Obj_Dot AABB_D2(AABB.x1, AABB.y2);
		cr_Obj_Dot AABB_D3(AABB.x2, AABB.y1);
		cr_Obj_Dot AABB_D4(AABB.x2, AABB.y2);

		if (IsContact(Tri_L1, AABB)){ return true; }
		if (IsContact(Tri_L2, AABB)){ return true; }
		if (IsContact(Tri_L3, AABB)){ return true; }

		//if (IsContact(Tri_D1, AABB)){ return true; }
		//if (IsContact(Tri_D2, AABB)){ return true; }
		//if (IsContact(Tri_D3, AABB)){ return true; }

		if (IsContact(Tri, AABB_D1)){ return true; }
		if (IsContact(Tri, AABB_D2)){ return true; }
		if (IsContact(Tri, AABB_D3)){ return true; }
		if (IsContact(Tri, AABB_D4)){ return true; }

		return false;
	}
	ISCONTACT_concept IsContact(const Object_Triangle<T1>& Tri, const Object_Circle<T2>& Cir)
	{
		using CheckHitImpl::add_cr;
		using CheckHitImpl::Dist2;
		using CheckHitImpl::Side;
		typedef Object_Dot<typename add_cr<T1>::type> cr_Obj_Dot;
		typedef Object_Line<typename add_cr<T1>::type> cr_Obj_Line;

		cr_Obj_Dot Tri_D1(Tri.x1, Tri.y1);
		cr_Obj_Dot Tri_D2(Tri.x2, Tri.y2);
		cr_Obj_Dot Tri_D3(Tri.x3, Tri.y3);
		cr_Obj_Line Tri_L1(Tri_D1, Tri_D2);
		cr_Obj_Line Tri_L2(Tri_D2, Tri_D3);
		cr_Obj_Line Tri_L3(Tri_D3, Tri_D1);


		//円と辺
		if (IsContact(Tri_L1, Cir)){ return true; }
		if (IsContact(Tri_L2, Cir)){ return true; }
		if (IsContact(Tri_L3, Cir)){ return true; }

		//中心と三角形
		cr_Obj_Dot R_cen(Cir.x, Cir.y);
		if (IsContact(Tri, R_cen)){ return true; }

		return  false;
	}


	//----------AABB to X ----------//

	ISCONTACT_concept_inline IsContact(const Object_AABB<T1>& AABB, const Object_Dot<T2>& Dot)
	{
		return IsContact(Dot, AABB);
	}
	ISCONTACT_concept_inline IsContact(const Object_AABB<T1>& AABB, const Object_Line<T2>& Line)
	{
		return IsContact(Line, AABB);
	}
	ISCONTACT_concept_inline IsContact(const Object_AABB<T1>& AABB, const Object_Triangle<T2>& Tri)
	{
		return IsContact(Tri, AABB);
	}
	ISCONTACT_concept_inline IsContact(const Object_AABB<T1>& AABB1, const Object_AABB<T2>& AABB2)
	{
		typedef typename CheckHitImpl::add_cr<T1>::type cr_T;
		typedef Object_AABB<cr_T> cr_Obj_AABB;
		return CheckOverlap<cr_T>(cr_Obj_AABB(AABB1), cr_Obj_AABB(AABB2));
	}
	ISCONTACT_concept_inline IsContact(const Object_AABB<T1>& AABB, const Object_Circle<T2>& Cir)
	{
		return IsContact(Cir, AABB);
	}


	//----------Circle to X ----------//

	ISCONTACT_concept_inline IsContact(const Object_Circle<T1>& Cir, const Object_Dot<T2>& Dot)
	{
		using CheckHitImpl::Dist2;
		typedef Object_Dot<typename CheckHitImpl::add_cr<T1>::type> clr_Obj_Dot;
		return (Cir.r*Cir.r >= Dist2(clr_Obj_Dot(Cir.x, Cir.y), Dot));
	}
	ISCONTACT_concept_inline IsContact(const Object_Circle<T1>& Cir, const Object_Line<T2>& Line)
	{
		return IsContact(Line, Cir);
	}
	ISCONTACT_concept_inline IsContact(const Object_Circle<T1>& Cir, const Object_Triangle<T2>& Tri)
	{
		return IsContact(Tri, Cir);
	}
	ISCONTACT_concept IsContact(const Object_Circle<T1>& Cir, const Object_AABB<T2>& AABB)
	{
		using CheckHitImpl::add_cr;
		using CheckHitImpl::Dist2;
		typedef Object_Dot<typename add_cr<T1>::type> cr_Obj_Dot;
		typedef Object_Line<typename add_cr<T1>::type> cr_Obj_Line;

		cr_Obj_Dot AABB_D1(AABB.x1, AABB.y1);
		cr_Obj_Dot AABB_D2(AABB.x1, AABB.y2);
		cr_Obj_Dot AABB_D3(AABB.x2, AABB.y1);
		cr_Obj_Dot AABB_D4(AABB.x2, AABB.y2);
		cr_Obj_Line AABB_L1(AABB_D1, AABB_D2);
		cr_Obj_Line AABB_L2(AABB_D2, AABB_D3);
		cr_Obj_Line AABB_L3(AABB_D3, AABB_D4);
		cr_Obj_Line AABB_L4(AABB_D4, AABB_D1);

		if (IsContact(Cir, AABB_L1)){ return true; }
		if (IsContact(Cir, AABB_L2)){ return true; }
		if (IsContact(Cir, AABB_L3)){ return true; }
		if (IsContact(Cir, AABB_L4)){ return true; }

		//if (IsContact(Cir, AABB_D1)){ return true; }
		//if (IsContact(Cir, AABB_D2)){ return true; }
		//if (IsContact(Cir, AABB_D3)){ return true; }
		//if (IsContact(Cir, AABB_D4)){ return true; }

		if (IsContact(cr_Obj_Dot(Cir.x, Cir.y), AABB)){ return true; }

		return false;
	}
	ISCONTACT_concept_inline IsContact(const Object_Circle<T1>& Cir1, const Object_Circle<T2>& Cir2)
	{
		using CheckHitImpl::add_cr;
		typedef typename add_cr<T1>::type cr_T;
		typedef Object_Dot<cr_T> cr_Obj_Dot;
		typedef Object_Circle<cr_T> cr_Obj_Cir;
		return IsContact(cr_Obj_Cir(Cir1.x, Cir1.y, Cir1.r + Cir2.r), cr_Obj_Dot(Cir2.x, Cir2.y));
	}


	//----------Polygon to X ----------//

	namespace CheckHitImpl
	{
		template<typename T1, typename Container1, typename Object2>
		inline auto IsContact_Polygon(const Object_Combined<T1, Container1>& polygon, const Object2& obj2
				, Type<Object_Combined<T1, Container1>>, Type<Object2>)
			->decltype(IsContact(*std::declval<typename Container1::value_type>(), obj2))
		{
			using std::begin; using std::end;
			return std::any_of(begin(polygon.objects), end(polygon.objects), [&](typename Container1::value_type const& obj1){
				return IsContact(*obj1, obj2);
			});
		};
		template<typename Object1, typename T2, typename Container2>
		inline auto IsContact_Polygon(const Object1& obj1, const Object_Combined<T2, Container2>& polygon
			, Type<Object1>, Type<Object_Combined<T2, Container2>>)
			->decltype(IsContact(polygon, obj1))
		{
			return IsContact(polygon, obj1);
		};
		template<typename T1, typename Container1, typename T2, typename Container2>
		inline auto IsContact_Polygon(const Object_Combined<T1, Container1>& polygon1, const Object_Combined<T2, Container2>& polygon2
				, Type<Object_Combined<T1, Container1>>, Type<Object_Combined<T2, Container2>>)
			->decltype(IsContact(polygon1, *std::declval<typename Container1::value_type>()))
		{
			using std::begin; using std::end;
			return std::any_of(begin(polygon1.objects), end(polygon1.objects), [&](typename Container1::value_type const& obj1){
				return IsContact(polygon2, *obj1);
			});
		};

		template<typename Object1, typename Object2>
		inline auto IsContact_Polygon_Run(const Object1& obj1, const Object2& obj2)
			->decltype(CheckHitImpl::IsContact_Polygon(obj1, obj2, CheckHitImpl::Type<Object1>(), CheckHitImpl::Type<Object2>()))
		{
			return CheckHitImpl::IsContact_Polygon(obj1, obj2, CheckHitImpl::Type<Object1>(), CheckHitImpl::Type<Object2>());
		}
	}


}
#include "CheckHit_ISCONTACTconcept_end.h"
#endif
