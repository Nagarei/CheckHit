// Win32Project1.cpp : アプリケーションのエントリ ポイントを定義します。
//
#include "stdafx.h"
#include "CheckHit.h"
#include <array>
#include <vector>

using namespace CheckHit;

using rational = boost::rational<int>;
typedef Object_Bace <rational> Obj_type;
const rational WORLD_SCALE = 2 * 3 * 5 * 7;

class Move_Obj_Bace
{
public:
	virtual std::unique_ptr<Move_Obj_Bace> CreateNext() = 0;
	virtual Obj_type& GetObj() = 0;
	virtual void Set(rational x, rational y) = 0;
	virtual void Draw(int color)const = 0;
	virtual ~Move_Obj_Bace(){}
};
class Move_Obj_Dot : public Move_Obj_Bace
{
	Object_Dot<rational> obj;
public:
	Move_Obj_Dot(rational x, rational y) : obj(x, y){}
	std::unique_ptr<Move_Obj_Bace> CreateNext()override;
	Obj_type& GetObj()override{ return obj; }
	void Set(rational x, rational y)override{ *this = Move_Obj_Dot(x,y); }
	void Draw(int color)const override{ DrawPixel(boost::rational_cast<int>(obj.x * WORLD_SCALE), boost::rational_cast<int>(obj.y * WORLD_SCALE), color); }
};
class Move_Obj_Line : public Move_Obj_Bace
{
	Object_Line<rational> obj;
	bool is_slanting = true;
public:
	Move_Obj_Line(rational x, rational y) : obj(x - 50 / WORLD_SCALE, y - 50 / WORLD_SCALE, x + 50 / WORLD_SCALE, y + 50 / WORLD_SCALE){}
	Move_Obj_Line(rational x, rational y, bool) : obj(x - 50 / WORLD_SCALE, y, x + 50 / WORLD_SCALE, y), is_slanting(false){}
	std::unique_ptr<Move_Obj_Bace> CreateNext()override;
	Obj_type& GetObj()override{ return obj; }
	void Set(rational x, rational y)override{ *this = is_slanting ? Move_Obj_Line(x, y) : Move_Obj_Line(x, y, false); }
	void Draw(int color)const override{ DrawLine(boost::rational_cast<int>(obj.x1 * WORLD_SCALE), boost::rational_cast<int>(obj.y1 * WORLD_SCALE), boost::rational_cast<int>(obj.x2 * WORLD_SCALE), boost::rational_cast<int>(obj.y2 * WORLD_SCALE), color); }
};
class Move_Obj_Triangle : public Move_Obj_Bace
{
	Object_Triangle<rational> obj;
public:
	Move_Obj_Triangle(rational x, rational y) : obj(x - 25 / WORLD_SCALE, y - 25 / WORLD_SCALE, x + 25 / WORLD_SCALE, y - 25 / WORLD_SCALE, x, y + 25 / WORLD_SCALE){}
	std::unique_ptr<Move_Obj_Bace> CreateNext()override;
	Obj_type& GetObj()override{ return obj; }
	void Set(rational x, rational y)override{ *this = Move_Obj_Triangle(x, y); }
	void Draw(int color)const override{ DrawTriangle(boost::rational_cast<int>(obj.x1 * WORLD_SCALE), boost::rational_cast<int>(obj.y1 * WORLD_SCALE), boost::rational_cast<int>(obj.x2 * WORLD_SCALE), boost::rational_cast<int>(obj.y2 * WORLD_SCALE), boost::rational_cast<int>(obj.x3 * WORLD_SCALE), boost::rational_cast<int>(obj.y3 * WORLD_SCALE), color, TRUE); }
};
class Move_Obj_AABB : public Move_Obj_Bace
{
	Object_AABB<rational> obj;
public:
	Move_Obj_AABB(rational x, rational y) : obj(x - 25 / WORLD_SCALE, y - 25 / WORLD_SCALE, x + 25 / WORLD_SCALE, y + 25 / WORLD_SCALE){}
	std::unique_ptr<Move_Obj_Bace> CreateNext()override;
	Obj_type& GetObj()override{ return obj; }
	void Set(rational x, rational y)override{ *this = Move_Obj_AABB(x, y); }
	void Draw(int color)const override{ DrawBox(boost::rational_cast<int>(obj.x1 * WORLD_SCALE), boost::rational_cast<int>(obj.y1 * WORLD_SCALE), boost::rational_cast<int>(obj.x2 * WORLD_SCALE), boost::rational_cast<int>(obj.y2 * WORLD_SCALE), color, TRUE); }
};
class Move_Obj_Circle : public Move_Obj_Bace
{
	Object_Circle<rational> obj;
public:
	Move_Obj_Circle(rational x, rational y) : obj(x, y, 25 / WORLD_SCALE){}
	std::unique_ptr<Move_Obj_Bace> CreateNext()override;
	Obj_type& GetObj()override{ return obj; }
	void Set(rational x, rational y)override{ *this = Move_Obj_Circle(x, y); }
	void Draw(int color)const override{ DrawCircle(boost::rational_cast<int>(obj.x * WORLD_SCALE), boost::rational_cast<int>(obj.y * WORLD_SCALE), boost::rational_cast<int>(obj.r * WORLD_SCALE), color, TRUE); }
};
class Move_Obj_Star : public Move_Obj_Bace
{
	std::unique_ptr<Object_Combined < rational
		, std::array < std::unique_ptr<Object_Triangle<rational>>, 3 >
		>> obj;
	rational cen_x, cen_y;
public:
	Move_Obj_Star(rational x, rational y) : cen_x(x), cen_y(y){ CreateObject(); }
	std::unique_ptr<Move_Obj_Bace> CreateNext()override;
	Obj_type& GetObj()override{ return *obj; }
	void Set(rational x, rational y)override{ obj.reset(); cen_x = (x); cen_y = (y); CreateObject(); }
	void Draw(int color)const override;
private:
	void CreateObject();
};

int APIENTRY _tWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPTSTR, _In_ int)
{
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1){ return -1; }
	SetDrawScreen(DX_SCREEN_BACK);

	SetMousePoint(320, 240);
	std::unique_ptr<Move_Obj_Bace> my_obj = std::make_unique<Move_Obj_Dot>(320 / WORLD_SCALE, 240 / WORLD_SCALE);
	Move_Obj_Dot Dot(100 / WORLD_SCALE, 100 / WORLD_SCALE);
	Move_Obj_Line Line(300 / WORLD_SCALE, 100 / WORLD_SCALE, false);
	Move_Obj_Triangle Triangle(500 / WORLD_SCALE, 100 / WORLD_SCALE);
	Move_Obj_AABB AABB(100 / WORLD_SCALE, 300 / WORLD_SCALE);
	Move_Obj_Circle Circle(300 / WORLD_SCALE, 300 / WORLD_SCALE);
	Move_Obj_Star Star(500 / WORLD_SCALE, 300 / WORLD_SCALE);

	int now_x, now_y;
	bool last_z = false;
	bool last_x = false;
	bool is_capture = true;
	while (ProcessMessage() == 0)
	{
		if (CheckHitKey(KEY_INPUT_Z)){
			if (last_z == false){
				my_obj = my_obj->CreateNext();
				last_z = true;
			}
		}
		else{
			last_z = false;
		}
		if (CheckHitKey(KEY_INPUT_X)){
			if (last_x == false){
				is_capture = !is_capture;
				last_x = true;
			}
		}
		else{
			last_x = false;
		}
		if (is_capture){
			GetMousePoint(&now_x, &now_y);
			my_obj->Set(now_x / WORLD_SCALE, now_y / WORLD_SCALE);
		}

		auto white = GetColor(255, 255, 255);
		auto red = GetColor(255, 0, 0);
		auto blue = GetColor(0, 0, 255);
		bool is_contact = false;
		
		//点
		if (IsContact(Dot.GetObj(), my_obj->GetObj())){
			is_contact = true;
			Dot.Draw(blue);
		}
		else{
			Dot.Draw(white);
		}
		//線
		if (IsContact(Line.GetObj(), my_obj->GetObj())){
			is_contact = true;
			Line.Draw(blue);
		}
		else{
			Line.Draw(white);
		}
		//三角
		if (IsContact(Triangle.GetObj(), my_obj->GetObj())){
			is_contact = true;
			Triangle.Draw(blue);
		}
		else{
			Triangle.Draw(white);
		}
		//四角
		if (IsContact(AABB.GetObj(), my_obj->GetObj())){
			is_contact = true;
			AABB.Draw(blue);
		}
		else{
			AABB.Draw(white);
		}
		//円
		if (IsContact(Circle.GetObj(), my_obj->GetObj())){
			is_contact = true;
			Circle.Draw(blue);
		}
		else{
			Circle.Draw(white);
		}
		//星
		if (IsContact(Star.GetObj(), my_obj->GetObj())){
			is_contact = true;
			Star.Draw(blue);
		}
		else{
			Star.Draw(white);
		}

		my_obj->Draw(is_contact ? red : white);

		ScreenFlip();
		ClearDrawScreen();
	}

	DxLib_End();
	return 0;
	//std::is_same <
	//	typename std::remove_cv<typename std::remove_reference<typename CheckHit::CheckHitImpl::remove_reference_wrapper<const double &>::type>::type>::type
	//	, typename std::remove_cv<typename std::remove_reference<typename CheckHit::CheckHitImpl::remove_reference_wrapper<std::reference_wrapper<const double>>::type>::type>::type
	//> ::value;
	CheckHit::CheckHitImpl::IsContact_Concept <const std::reference_wrapper<const double>&
		, std::reference_wrapper<const double> >::value;
	CheckHit::CheckHitImpl::remove_crv_wrapper < std::reference_wrapper<const double> > ;

	std::unique_ptr<Object_Combined < rational
		, std::array < std::unique_ptr<Object_Triangle<rational>>, 3 >
		>> obj_test1;
	std::unique_ptr<Object_Combined < rational
		, std::vector < std::unique_ptr<Object_Dot<rational>> >
		>> obj_test2;
	IsContact(*obj_test1, *obj_test2);
	IsContact(*obj_test1, *obj_test1);
	IsContact(*obj_test2, *obj_test2);
	IsContact(*obj_test1, Object_Dot<rational>());
}

std::unique_ptr<Move_Obj_Bace> Move_Obj_Dot::CreateNext()
{
	return std::make_unique<Move_Obj_Line>(obj.x, obj.y);
}
std::unique_ptr<Move_Obj_Bace> Move_Obj_Line::CreateNext()
{
	return std::make_unique<Move_Obj_Triangle>((obj.x1 + obj.x2) / 2, (obj.y1 + obj.y2) / 2);
}
std::unique_ptr<Move_Obj_Bace> Move_Obj_Triangle::CreateNext()
{
	return std::make_unique<Move_Obj_AABB>((obj.x1 + obj.x2 + obj.x3) / 3, (obj.y1 + obj.y2 + obj.y3) / 3);
}
std::unique_ptr<Move_Obj_Bace> Move_Obj_AABB::CreateNext()
{
	return std::make_unique<Move_Obj_Circle>((obj.x1 + obj.x2) / 2, (obj.y1 + obj.y2) / 2);
}
std::unique_ptr<Move_Obj_Bace> Move_Obj_Circle::CreateNext()
{
	return std::make_unique<Move_Obj_Star>(obj.x, obj.y);
}
std::unique_ptr<Move_Obj_Bace> Move_Obj_Star::CreateNext()
{
	return std::make_unique<Move_Obj_Dot>(cen_x, cen_y);
}


void Move_Obj_Star::CreateObject()
{
	std::array < std::unique_ptr<Object_Triangle<rational>>, 3 > objects = {
		std::make_unique<Object_Triangle<rational>>(cen_x - (50  / WORLD_SCALE), cen_y - (12  / WORLD_SCALE), cen_x + (12  / WORLD_SCALE), cen_y - (12  / WORLD_SCALE), cen_x + (30  / WORLD_SCALE), cen_y + (50  / WORLD_SCALE))
		, std::make_unique<Object_Triangle<rational>>(cen_x + (50  / WORLD_SCALE), cen_y - (12  / WORLD_SCALE), cen_x - (12  / WORLD_SCALE), cen_y - (12  / WORLD_SCALE), cen_x - (30  / WORLD_SCALE), cen_y + (50  / WORLD_SCALE))
		, std::make_unique<Object_Triangle<rational>>(cen_x, cen_y - (50  / WORLD_SCALE), cen_x - (12  / WORLD_SCALE), cen_y - (12  / WORLD_SCALE), cen_x + (12  / WORLD_SCALE), cen_y - (12  / WORLD_SCALE))
	};
	obj = std::make_unique < Object_Combined < rational
		, std::array < std::unique_ptr<Object_Triangle<rational>>, 3 >
		>>
		();
	std::move(objects.begin(), objects.end(), obj->objects.begin());
}
void Move_Obj_Star::Draw(int color)const
{
	for (auto& i : obj->objects)
	{
		DrawTriangle(boost::rational_cast<int>(i->x1 * WORLD_SCALE), boost::rational_cast<int>(i->y1 * WORLD_SCALE)
			, boost::rational_cast<int>(i->x2 * WORLD_SCALE), boost::rational_cast<int>(i->y2 * WORLD_SCALE)
			, boost::rational_cast<int>(i->x3 * WORLD_SCALE), boost::rational_cast<int>(i->y3 * WORLD_SCALE)
			, color, TRUE);
	}
}
