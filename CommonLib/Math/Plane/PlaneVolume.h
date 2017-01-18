#pragma once
#include "..\Vector\Vector.h"
#include "Plane.h"
#include "..\Collision\Sphere.h"
#include "../../DataStructures/StaticArray.h"
namespace CommonUtilities
{
		template<typename TYPE>
		class PlaneVolume
		{
		public:
			PlaneVolume() = default;
			PlaneVolume(CU::StaticArray<Plane<TYPE>, 6> plane_list);

			void AddPlane(Plane<TYPE> aPlane);
			void RemovePlane(Plane<TYPE> aPlane);

			bool Inside(Math::Vector3<TYPE> aPosition);
			//bool Inside(Intersection::Sphere aSphere);

		private:
			CU::StaticArray<Plane<TYPE>, 6> m_Planes;
		};

		template<typename TYPE>
		PlaneVolume<TYPE>::PlaneVolume(CU::StaticArray<Plane<TYPE>, 6> plane_list)
		{
			m_Planes = plane_list;
		}

		template<typename TYPE>
		void PlaneVolume<TYPE>::AddPlane(Plane<TYPE> aPlane)
		{
			m_Planes.InsertLast(aPlane);
		}

		template<typename TYPE>
		void PlaneVolume<TYPE>::RemovePlane(Plane<TYPE> aPlane)
		{
			//myPlanes.RemoveCyclic(aPlane);
		}

		template<typename TYPE>
		bool PlaneVolume<TYPE>::Inside(Math::Vector3<TYPE> aPosition)
		{
			for (Plane<TYPE>& plane : m_Planes )
			{
				if (!plane.Inside(aPosition))
					return false;
			}

			return true;
		}

		//template <typename TYPE>
		//bool PlaneVolume<TYPE>::Inside(Intersection::Sphere aSphere)
		//{
		//	PlaneVolume<TYPE> planes;

		//	/*for (unsigned int i = 0; i < myPlanes.size(); i++)
		//	{
		//		Plane<TYPE> plane(myPlanes[i]);
		//		plane.IncreaseDistance(aSphere.myRadius);
		//		planes.AddPlane(plane);
		//	}*/

		//	//return planes.Inside(aSphere.myCenterPosition);
		//}
};
namespace CU = CommonUtilities;