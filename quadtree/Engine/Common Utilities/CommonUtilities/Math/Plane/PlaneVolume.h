#pragma once
#include <vector>
#include "..\Vector\Vector.h"
#include "Plane.h"
#include "..\Collision\Sphere.h"
namespace CommonUtilities
{
	namespace Math
	{
		template<typename TYPE>
		class PlaneVolume
		{
		public:
			PlaneVolume();
			PlaneVolume(std::vector<Plane<TYPE>> aPlaneList);
			~PlaneVolume();

			void AddPlane(Plane<TYPE> aPlane);
			void RemovePlane(Plane<TYPE> aPlane);

			bool Inside(Vector3<TYPE> aPosition);
			bool Inside(Intersection::Sphere aSphere);

		private:

			std::vector<Plane<TYPE>> myPlanes;

		};

		template<typename TYPE>
		PlaneVolume<TYPE>::PlaneVolume()
		{
		}

		template<typename TYPE>
		PlaneVolume<TYPE>::~PlaneVolume()
		{
		}

		template<typename TYPE>
		PlaneVolume<TYPE>::PlaneVolume(std::vector<Plane<TYPE>> aPlaneList)
		{
			myPlanes = aPlaneList;
		}

		template<typename TYPE>
		void PlaneVolume<TYPE>::AddPlane(Plane<TYPE> aPlane)
		{
			myPlanes.push_back(aPlane);
		}

		template<typename TYPE>
		void PlaneVolume<TYPE>::RemovePlane(Plane<TYPE> aPlane)
		{
			myPlanes.RemoveCyclic(aPlane);
		}

		template<typename TYPE>
		bool PlaneVolume<TYPE>::Inside(Vector3<TYPE> aPosition)
		{
			for (unsigned int i = 0; i < myPlanes.size(); ++i)
			{
				if (myPlanes[i].Inside(aPosition) == false)
					return false;
			}
			return true;
		}

		template <typename TYPE>
		bool PlaneVolume<TYPE>::Inside(Intersection::Sphere aSphere)
		{
			PlaneVolume<TYPE> planes;

			for (unsigned int i = 0; i < myPlanes.size(); i++)
			{
				Plane<TYPE> plane(myPlanes[i]);
				plane.IncreaseDistance(aSphere.myRadius);
				planes.AddPlane(plane);
			}

			return planes.Inside(aSphere.myCenterPosition);
		}

	};
};
namespace CU = CommonUtilities;