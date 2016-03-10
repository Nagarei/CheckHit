#ifndef SQUEARE_HPP_201506191635416510631564
#define SQUEARE_HPP_201506191635416510631564
#include <utility>
#include <cmath>
#ifdef _MSC_VER
#pragma warning( disable : 4512 )
#endif
namespace CheckHit
{

	//Tには標準の符号付整数or浮動点小数型が入れられることを想定しています
	template<typename T>
	struct Square_Point//座標
	{
		T Top_Side;	//上辺
		T Bottom_Side;	//底辺
		T Right_Side;	//右辺
		T Left_Side;	//左辺

		Square_Point(const T& top, const T& bottom, const T& right, const T& left)
			:Top_Side(top)
			, Bottom_Side(bottom)
			, Right_Side(right)
			, Left_Side(left)
		{}
		Square_Point(void){}
	};

	//Tには標準の符号付整数or浮動点小数型が入れられることを想定しています
	template<typename T>
	struct Move_Vector//ベクトル
	{
		T x;
		T y;
		Move_Vector(const T& param_x, const T& param_y)
			:x(param_x)
			, y(param_y)
		{}
		Move_Vector(void){}
	};

	//------------------------------------------extern宣言-------------------------------------------------------//

	//重なっていたらtrueを返す
	template<typename T>
	extern bool CheckOverlap(const Square_Point<T>& obj1, const Square_Point<T>& obj2);

	//playerをobj1もとにどれくらいうごかせばいいか計算する
	//friction:摩擦力っぽいもの。これに負数を入れると無限として扱われます。
	template<typename T>
	extern Move_Vector<T> CalculateMove(Square_Point<T> player, const Move_Vector<T>& player_vec, const Square_Point<T>& obj1, const T& friction_x, const T& friction_y);

	//中心座標と幅、高さを指定してSquare_Pointを作る
	template<typename T>
	extern Square_Point<T> SquareFromCenterPoint(const T& x, const T& y, const T& width, const T& height);

	//左上座標と幅、高さを指定してSquare_Pointを作る
	template<typename T>
	extern Square_Point<T> SquareFromUpperLeftPoint(const T& x, const T& y, const T& width, const T& height);

	//中心下座標と幅、高さを指定してSquare_Pointを作る
	template<typename T>
	extern Square_Point<T> SquareFromBottomCenterPoint(const T& x, const T& y, const T& width, const T& height);








	//----------------------------------定義---------------------------------------------//

	//重なっていたらtrueを返す
	template<typename T>
	inline bool CheckOverlap(const Square_Point<T>& obj1, const Square_Point<T>& obj2)
	{
		//obj2はobj1の
		if (((obj1.Right_Side >= obj2.Left_Side) &&	//　　右端より左にあり
			(obj1.Left_Side <= obj2.Right_Side)) &&	//かつ左端より右にあり
			((obj1.Top_Side <= obj2.Bottom_Side) &&	//かつ上端より下にあり
			(obj1.Bottom_Side >= obj2.Top_Side))){ //かつ下端より上にあるとき
			return true;
		}
		else{
			return false;
		}
	}

	//playerをobj1もとにどれくらいうごかせばいいか計算する
	//friction:摩擦力っぽいもの。
	template<typename T, typename FLOAT = double>
	Move_Vector<T> CalculateMove(Square_Point<T> player, const Move_Vector<T>& player_vec, const Square_Point<T>& obj1, const T& friction_x, const T& friction_y)
	{
		using std::abs;
		using std::min;
		Move_Vector<T> result(0, 0);
		if (CheckOverlap(player, obj1)){ //当たっているとき

			//左右に補正する
			if (player_vec.x > 0){//右に動いてるときは左に
				result.x = obj1.Left_Side - player.Right_Side - 1;
			}
			else if (player_vec.x < 0){//左に動いてるときは右に
				result.x = obj1.Right_Side - player.Left_Side + 1;
			}

			//上下に補正する
			if (player_vec.y>0){//下に動いてるときは上に
				result.y = obj1.Top_Side - player.Bottom_Side - 1;
			}
			else if (player_vec.y < 0){//上に動いてるときは下に//
				result.y = obj1.Bottom_Side - player.Top_Side + 1;
			}

			if (player_vec.x != 0 && player_vec.y != 0){//斜めに補正する

				const T&& absolute_result_y_based_x(static_cast<T>(abs(FLOAT(result.x) / FLOAT(player_vec.x)*FLOAT(player_vec.y))/*+0.5*/));
				if (abs(result.y) > absolute_result_y_based_x){
					//xを優先するとyを優先した場合未満の距離で重なりを解消できるときxを優先する
					//摩擦の計算
					if (player_vec.y > 0){
						result.y = -min(friction_y, absolute_result_y_based_x);
					}
					else{
						result.y = min(friction_y, absolute_result_y_based_x);
					}
				}
				else{
					//yを優先しx以下の距離で重なりを解消する
					//摩擦の計算
					const T&& absolute_result_x_based_y(static_cast<T>(abs(FLOAT(result.y) / FLOAT(player_vec.y)*FLOAT(player_vec.x))/*+0.5*/));
					if (player_vec.x > 0){
						result.x = -min(friction_x, absolute_result_x_based_y);
					}
					else{
						result.x = min(friction_x, absolute_result_x_based_y);
					}
				}
			}
		}
		return result;
	}

	//中心座標と幅、高さを指定してSquare_Pointを作る
	template<typename T>
	inline Square_Point<T> SquareFromCenterPoint(const T& x, const T& y, const T& width, const T& height)
	{
		return Square_Point<T>(
			T(y - (height - 1.0) / 2.0)
			, T(y + (height - 1.0) / 2.0)
			, T(x + (width - 1.0) / 2.0)
			, T(x - (width - 1.0) / 2.0)
			);
	}

	//左上座標と幅、高さを指定してSquare_Pointを作る
	template<typename T>
	inline Square_Point<T> SquareFromUpperLeftPoint(const T& x, const T& y, const T& width, const T& height)
	{
		return Square_Point<T>(
			T(y)
			, T(y + height - 1.0)
			, T(x + width - 1.0)
			, T(x)
			);
	}

	//真ん中下座標と幅、高さを指定してSquare_Pointを作る
	template<typename T>
	inline Square_Point<T> SquareFromBottomCenterPoint(const T& x, const T& y, const T& width, const T& height)
	{
		return Square_Point<T>(
			T(y - height)
			, T(y)
			, T(x + (width - 1) / 2)
			, T(x - (width - 1) / 2)
			);
	}


}
#endif
