#ifndef CHECK_HIT_ISCONTACT_CONSEPT_H_201506192111156516
#define CHECK_HIT_ISCONTACT_CONSEPT_H_201506192111156516
#include <type_traits>

namespace CheckHit
{
	namespace CheckHitImpl
	{
		template<typename T>
		struct remove_reference_wrapper
		{
			typedef T type;
		};
		template<typename T>
		struct remove_reference_wrapper<std::reference_wrapper<T>>
		{
			typedef typename std::add_lvalue_reference<T>::type type;
		};

		template<typename T_b, typename n_T = typename remove_reference_wrapper<typename std::remove_cv<typename std::remove_reference<T_b>::type>::type>::type>
		struct remove_crv_wrapper
			: remove_crv_wrapper<n_T>
		{};
		template<typename T>
		struct remove_crv_wrapper<T, T>
		{
			typedef T type;
		};

		template<typename T1, typename T2>
		struct IsContact_Concept
			: std::is_same <
			typename remove_crv_wrapper<T1>::type
			, typename remove_crv_wrapper<T2>::type
			>
		{};
	}
}
//インクルードガード
#endif

#define ISCONTACT_tempalte(template_param) ,##template_param
#define ISCONTACT_tempalte_type(template_param) ISCONTACT_tempalte(typename template_param)

#define ISCONTACT_concept_main(option,add_template) \
	template<typename T1, typename T2 add_template>\
	option typename std::enable_if<CheckHitImpl::IsContact_Concept<T1, T2>::value, bool>::type

//IsContact関数は両辺の元の型が同じなら定義する
#define ISCONTACT_concept ISCONTACT_concept_main(,)
//IsContact関数は両辺の元の型が同じなら定義する
#define ISCONTACT_concept_inline ISCONTACT_concept_main(inline,)

#define OBJECT_operator_equal_concept(return_type) \
	typename std::enable_if<std::is_assignable<T, T2>::value, return_type>::type
#define OBJECT_operator_equal_with_concept_main(type, option1, option2) \
	template<typename T2>\
	option1 OBJECT_operator_equal_concept(type<T>&)\
		option2 operator=(const type<T2>& obj2)
#define OBJECT_operator_equal_with_concept(type) OBJECT_operator_equal_with_concept_main(type,,)
#define OBJECT_operator_equal_with_concept_define(type) template<typename T> OBJECT_operator_equal_with_concept_main(type,inline,type<T>::)

