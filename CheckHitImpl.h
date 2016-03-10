#ifndef CHECK_HIT_IMPL_H_201506192037
#define CHECK_HIT_IMPL_H_201506192037
#include "CheckHit.h"

#include "CheckHit_ISCONTACTconcept.h"
namespace CheckHit
{
	
	ISCONTACT_concept IsContact(const Object_Dot<T1>& Dot1, const Object_Dot<T2>&      Dot2);
	ISCONTACT_concept IsContact(const Object_Dot<T1>& Dot , const Object_Line<T2>&     Line);
	ISCONTACT_concept IsContact(const Object_Dot<T1>& Dot , const Object_Triangle<T2>& Tri);
	ISCONTACT_concept IsContact(const Object_Dot<T1>& Dot , const Object_AABB<T2>&     AABB);
	ISCONTACT_concept IsContact(const Object_Dot<T1>& Dot , const Object_Circle<T2>&   Cir);
	ISCONTACT_concept IsContact(const Object_Line<T1>& Line , const Object_Dot<T2>&      Dot);
	ISCONTACT_concept IsContact(const Object_Line<T1>& Line1, const Object_Line<T2>&     Line2);
	ISCONTACT_concept IsContact(const Object_Line<T1>& Line , const Object_Triangle<T2>& Tri);
	ISCONTACT_concept IsContact(const Object_Line<T1>& Line , const Object_AABB<T2>&     AABB);
	ISCONTACT_concept IsContact(const Object_Line<T1>& Line , const Object_Circle<T2>&   Cir);
	ISCONTACT_concept IsContact(const Object_Triangle<T1>& Tri , const Object_Dot<T2>&      Dot);
	ISCONTACT_concept IsContact(const Object_Triangle<T1>& Tri , const Object_Line<T2>&     Line);
	ISCONTACT_concept IsContact(const Object_Triangle<T1>& Tri1, const Object_Triangle<T2>& Tri2);
	ISCONTACT_concept IsContact(const Object_Triangle<T1>& Tri , const Object_AABB<T2>&     AABB);
	ISCONTACT_concept IsContact(const Object_Triangle<T1>& Tri , const Object_Circle<T2>&   Cir);
	ISCONTACT_concept IsContact(const Object_AABB<T1>& AABB , const Object_Dot<T2>&      Dot);
	ISCONTACT_concept IsContact(const Object_AABB<T1>& AABB , const Object_Line<T2>&     Line);
	ISCONTACT_concept IsContact(const Object_AABB<T1>& AABB , const Object_Triangle<T2>& Tri);
	ISCONTACT_concept IsContact(const Object_AABB<T1>& AABB1, const Object_AABB<T2>&     AABB2);
	ISCONTACT_concept IsContact(const Object_AABB<T1>& AABB , const Object_Circle<T2>&   Cir);
	ISCONTACT_concept IsContact(const Object_Circle<T1>& Cir , const Object_Dot<T2>&      Dot);
	ISCONTACT_concept IsContact(const Object_Circle<T1>& Cir , const Object_Line<T2>&     Line);
	ISCONTACT_concept IsContact(const Object_Circle<T1>& Cir , const Object_Triangle<T2>& Tri);
	ISCONTACT_concept IsContact(const Object_Circle<T1>& Cir , const Object_AABB<T2>&     AABB);
	ISCONTACT_concept IsContact(const Object_Circle<T1>& Cir1, const Object_Circle<T2>&   Cir2);

	//-----Object_CombinedÇÕëºÇ∑Ç◊ÇƒÇÃObjectÇ∆ìñÇΩÇËîªíËÇçsÇ¶ÇÈ
	namespace CheckHitImpl
	{
		template<typename>
		struct Type{};

		template<typename T1, typename Container1, typename Object2>
		auto IsContact_Polygon(const Object_Combined<T1, Container1>& polygon, const Object2& obj2
			, Type<Object_Combined<T1, Container1>>, Type<Object2>)
			->decltype(IsContact(*std::declval<typename Container1::value_type>(), obj2));
		template<typename Object1, typename T2, typename Container2>
		auto IsContact_Polygon(const Object1& obj1, const Object_Combined<T2, Container2>& polygon
			, Type<Object1>, Type<Object_Combined<T2, Container2>>)
			->decltype(IsContact(polygon, obj1));
		template<typename T1, typename Container1, typename T2, typename Container2>
		auto IsContact_Polygon(const Object_Combined<T1, Container1>& polygon1, const Object_Combined<T2, Container2>& polygon2
			, Type<Object_Combined<T1, Container1>>, Type<Object_Combined<T2, Container2>>)
			->decltype(IsContact(polygon1, *std::declval<typename Container1::value_type>()));
		template<typename Object1, typename Object2>
		auto IsContact_Polygon_Run(const Object1& obj1, const Object2& obj2)
			->decltype(CheckHitImpl::IsContact_Polygon(obj1, obj2, CheckHitImpl::Type<Object1>(), CheckHitImpl::Type<Object2>()));
	}
	
	ISCONTACT_concept_main(inline,ISCONTACT_tempalte_type(Conta2)) IsContact(const Object_Bace<T1>&     obj1, const Object_Combined<T2, Conta2>& Polygon){ return IsContact(Polygon, obj1); }
	ISCONTACT_concept_main(inline,ISCONTACT_tempalte_type(Conta2)) IsContact(const Object_Dot<T1>&      Dot , const Object_Combined<T2, Conta2>& Polygon){ return IsContact(Polygon, Dot ); }
	ISCONTACT_concept_main(inline,ISCONTACT_tempalte_type(Conta2)) IsContact(const Object_Line<T1>&     Line, const Object_Combined<T2, Conta2>& Polygon){ return IsContact(Polygon, Line); }
	ISCONTACT_concept_main(inline,ISCONTACT_tempalte_type(Conta2)) IsContact(const Object_Triangle<T1>& Tri , const Object_Combined<T2, Conta2>& Polygon){ return IsContact(Polygon, Tri ); }
	ISCONTACT_concept_main(inline,ISCONTACT_tempalte_type(Conta2)) IsContact(const Object_AABB<T1>&     AABB, const Object_Combined<T2, Conta2>& Polygon){ return IsContact(Polygon, AABB); }
	ISCONTACT_concept_main(inline,ISCONTACT_tempalte_type(Conta2)) IsContact(const Object_Circle<T1>&   Cir , const Object_Combined<T2, Conta2>& Polygon){ return IsContact(Polygon, Cir ); }
	ISCONTACT_concept_main(inline,ISCONTACT_tempalte_type(Conta1)) IsContact(const Object_Combined<T1, Conta1>& Polygon, const Object_Bace<T2>&     obj2){ return CheckHitImpl::IsContact_Polygon_Run(Polygon, obj2); }
	ISCONTACT_concept_main(inline,ISCONTACT_tempalte_type(Conta1)) IsContact(const Object_Combined<T1, Conta1>& Polygon, const Object_Dot<T2>&      Dot ){ return CheckHitImpl::IsContact_Polygon_Run(Polygon, Dot ); }
	ISCONTACT_concept_main(inline,ISCONTACT_tempalte_type(Conta1)) IsContact(const Object_Combined<T1, Conta1>& Polygon, const Object_Line<T2>&     Line){ return CheckHitImpl::IsContact_Polygon_Run(Polygon, Line); }
	ISCONTACT_concept_main(inline,ISCONTACT_tempalte_type(Conta1)) IsContact(const Object_Combined<T1, Conta1>& Polygon, const Object_Triangle<T2>& Tri ){ return CheckHitImpl::IsContact_Polygon_Run(Polygon, Tri ); }
	ISCONTACT_concept_main(inline,ISCONTACT_tempalte_type(Conta1)) IsContact(const Object_Combined<T1, Conta1>& Polygon, const Object_AABB<T2>&     AABB){ return CheckHitImpl::IsContact_Polygon_Run(Polygon, AABB); }
	ISCONTACT_concept_main(inline,ISCONTACT_tempalte_type(Conta1)) IsContact(const Object_Combined<T1, Conta1>& Polygon, const Object_Circle<T2>&   Cir ){ return CheckHitImpl::IsContact_Polygon_Run(Polygon, Cir ); }
	ISCONTACT_concept_main(inline,ISCONTACT_tempalte_type(Conta1)ISCONTACT_tempalte_type(Conta2)) IsContact(const Object_Combined<T1, Conta1>& Polygon1, const Object_Combined<T2, Conta2>& Polygon2){ return CheckHitImpl::IsContact_Polygon_Run(Polygon1, Polygon2); }


}
#include "CheckHit_ISCONTACTconcept_end.h"

#include "CheckHitImpl.hpp"
#endif
