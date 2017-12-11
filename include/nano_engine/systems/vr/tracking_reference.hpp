#ifndef NANO_ENGINE_SYSTEMS_VR_TRACKING_REFERENCE_HPP_
#define NANO_ENGINE_SYSTEMS_VR_TRACKING_REFERENCE_HPP_

#include <openvr.h>

#include <nano_engine/systems/vr/tracking_device.hpp>
#include <nano_engine/utility/rectangle.hpp>

namespace ne
{
class tracking_reference : public tracking_device<tracking_device_type::tracking_reference>
{
public:
  explicit tracking_reference  (const std::uint32_t& index) : tracking_device<tracking_device_type::tracking_reference>(index)
  {
  
  }
  tracking_reference           (const tracking_reference&  that) = default;
  tracking_reference           (      tracking_reference&& temp) = default;
  virtual ~tracking_reference  ()                                = default;
  tracking_reference& operator=(const tracking_reference&  that) = default;
  tracking_reference& operator=(      tracking_reference&& temp) = default;

  // IVR System - Property
  rectangle<float>     field_of_view () const
  {
    return rectangle<float>  
    {
      get_property_float(vr::Prop_FieldOfViewLeftDegrees_Float  ),
      get_property_float(vr::Prop_FieldOfViewRightDegrees_Float ),
      get_property_float(vr::Prop_FieldOfViewTopDegrees_Float   ),
      get_property_float(vr::Prop_FieldOfViewBottomDegrees_Float)
    };
  }
  std::array<float, 2> tracking_range() const
  {
    return std::array<float, 2> 
    {
      get_property_float(vr::Prop_TrackingRangeMinimumMeters_Float),
      get_property_float(vr::Prop_TrackingRangeMaximumMeters_Float)
    };
  }
  std::string          mode_label    () const
  {
    return get_property_string(vr::Prop_ModeLabel_String);
  }
};
}

#endif
