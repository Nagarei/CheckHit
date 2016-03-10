#ifndef SQUEARE_HPP_201506191635416510631564
#define SQUEARE_HPP_201506191635416510631564
#include <utility>
#include <cmath>
#ifdef _MSC_VER
#pragma warning( disable : 4512 )
#endif
namespace CheckHit
{

	//T�ɂ͕W���̕����t����or�����_�����^��������邱�Ƃ�z�肵�Ă��܂�
	template<typename T>
	struct Square_Point//���W
	{
		T Top_Side;	//���
		T Bottom_Side;	//���
		T Right_Side;	//�E��
		T Left_Side;	//����

		Square_Point(const T& top, const T& bottom, const T& right, const T& left)
			:Top_Side(top)
			, Bottom_Side(bottom)
			, Right_Side(right)
			, Left_Side(left)
		{}
		Square_Point(void){}
	};

	//T�ɂ͕W���̕����t����or�����_�����^��������邱�Ƃ�z�肵�Ă��܂�
	template<typename T>
	struct Move_Vector//�x�N�g��
	{
		T x;
		T y;
		Move_Vector(const T& param_x, const T& param_y)
			:x(param_x)
			, y(param_y)
		{}
		Move_Vector(void){}
	};

	//------------------------------------------extern�錾-------------------------------------------------------//

	//�d�Ȃ��Ă�����true��Ԃ�
	template<typename T>
	extern bool CheckOverlap(const Square_Point<T>& obj1, const Square_Point<T>& obj2);

	//player��obj1���Ƃɂǂꂭ�炢���������΂������v�Z����
	//friction:���C�͂��ۂ����́B����ɕ���������Ɩ����Ƃ��Ĉ����܂��B
	template<typename T>
	extern Move_Vector<T> CalculateMove(Square_Point<T> player, const Move_Vector<T>& player_vec, const Square_Point<T>& obj1, const T& friction_x, const T& friction_y);

	//���S���W�ƕ��A�������w�肵��Square_Point�����
	template<typename T>
	extern Square_Point<T> SquareFromCenterPoint(const T& x, const T& y, const T& width, const T& height);

	//������W�ƕ��A�������w�肵��Square_Point�����
	template<typename T>
	extern Square_Point<T> SquareFromUpperLeftPoint(const T& x, const T& y, const T& width, const T& height);

	//���S�����W�ƕ��A�������w�肵��Square_Point�����
	template<typename T>
	extern Square_Point<T> SquareFromBottomCenterPoint(const T& x, const T& y, const T& width, const T& height);








	//----------------------------------��`---------------------------------------------//

	//�d�Ȃ��Ă�����true��Ԃ�
	template<typename T>
	inline bool CheckOverlap(const Square_Point<T>& obj1, const Square_Point<T>& obj2)
	{
		//obj2��obj1��
		if (((obj1.Right_Side >= obj2.Left_Side) &&	//�@�@�E�[��荶�ɂ���
			(obj1.Left_Side <= obj2.Right_Side)) &&	//�����[���E�ɂ���
			((obj1.Top_Side <= obj2.Bottom_Side) &&	//����[��艺�ɂ���
			(obj1.Bottom_Side >= obj2.Top_Side))){ //�����[����ɂ���Ƃ�
			return true;
		}
		else{
			return false;
		}
	}

	//player��obj1���Ƃɂǂꂭ�炢���������΂������v�Z����
	//friction:���C�͂��ۂ����́B
	template<typename T, typename FLOAT = double>
	Move_Vector<T> CalculateMove(Square_Point<T> player, const Move_Vector<T>& player_vec, const Square_Point<T>& obj1, const T& friction_x, const T& friction_y)
	{
		using std::abs;
		using std::min;
		Move_Vector<T> result(0, 0);
		if (CheckOverlap(player, obj1)){ //�������Ă���Ƃ�

			//���E�ɕ␳����
			if (player_vec.x > 0){//�E�ɓ����Ă�Ƃ��͍���
				result.x = obj1.Left_Side - player.Right_Side - 1;
			}
			else if (player_vec.x < 0){//���ɓ����Ă�Ƃ��͉E��
				result.x = obj1.Right_Side - player.Left_Side + 1;
			}

			//�㉺�ɕ␳����
			if (player_vec.y>0){//���ɓ����Ă�Ƃ��͏��
				result.y = obj1.Top_Side - player.Bottom_Side - 1;
			}
			else if (player_vec.y < 0){//��ɓ����Ă�Ƃ��͉���//
				result.y = obj1.Bottom_Side - player.Top_Side + 1;
			}

			if (player_vec.x != 0 && player_vec.y != 0){//�΂߂ɕ␳����

				const T&& absolute_result_y_based_x(static_cast<T>(abs(FLOAT(result.x) / FLOAT(player_vec.x)*FLOAT(player_vec.y))/*+0.5*/));
				if (abs(result.y) > absolute_result_y_based_x){
					//x��D�悷���y��D�悵���ꍇ�����̋����ŏd�Ȃ�������ł���Ƃ�x��D�悷��
					//���C�̌v�Z
					if (player_vec.y > 0){
						result.y = -min(friction_y, absolute_result_y_based_x);
					}
					else{
						result.y = min(friction_y, absolute_result_y_based_x);
					}
				}
				else{
					//y��D�悵x�ȉ��̋����ŏd�Ȃ����������
					//���C�̌v�Z
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

	//���S���W�ƕ��A�������w�肵��Square_Point�����
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

	//������W�ƕ��A�������w�肵��Square_Point�����
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

	//�^�񒆉����W�ƕ��A�������w�肵��Square_Point�����
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
