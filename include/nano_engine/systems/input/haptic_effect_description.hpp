#ifndef NANO_ENGINE_SYSTEMS_INPUT_HAPTIC_EFFECT_DESCRIPTION_HPP_
#define NANO_ENGINE_SYSTEMS_INPUT_HAPTIC_EFFECT_DESCRIPTION_HPP_

#include <array>
#include <cstddef>
#include <vector>

#include <SDL2/SDL_haptic.h>

namespace ne
{
struct haptic_effect_description
{
  virtual ~haptic_effect_description()       = default;
  virtual SDL_HapticEffect native   () const = 0;
};

struct haptic_effect_description_condition : haptic_effect_description
{
  enum class type
  {
    spring   = SDL_HAPTIC_SPRING  ,
    damper   = SDL_HAPTIC_DAMPER  ,
    intertia = SDL_HAPTIC_INERTIA ,
    friction = SDL_HAPTIC_FRICTION
  };

  SDL_HapticEffect native() const override
  {
    SDL_HapticEffect native;
    native.condition.type           = static_cast<unsigned short>(type                 );
    native.condition.length         = static_cast<unsigned int>  (length               );
    native.condition.delay          = static_cast<unsigned short>(delay                );
    native.condition.button         = static_cast<unsigned short>(button               );
    native.condition.interval       = static_cast<unsigned short>(button_interval      );
    native.condition.right_sat  [0] = static_cast<unsigned short>(level_right       [0]);
    native.condition.right_sat  [1] = static_cast<unsigned short>(level_right       [1]);
    native.condition.right_sat  [2] = static_cast<unsigned short>(level_right       [2]);
    native.condition.left_sat   [0] = static_cast<unsigned short>(level_left        [0]);
    native.condition.left_sat   [1] = static_cast<unsigned short>(level_left        [1]);
    native.condition.left_sat   [2] = static_cast<unsigned short>(level_left        [2]);
    native.condition.right_coeff[0] = static_cast<unsigned short>(acceleration_right[0]);
    native.condition.right_coeff[1] = static_cast<unsigned short>(acceleration_right[1]);
    native.condition.right_coeff[2] = static_cast<unsigned short>(acceleration_right[2]);
    native.condition.left_coeff [0] = static_cast<unsigned short>(acceleration_left [0]);
    native.condition.left_coeff [1] = static_cast<unsigned short>(acceleration_left [1]);
    native.condition.left_coeff [2] = static_cast<unsigned short>(acceleration_left [2]);
    native.condition.deadband   [0] = static_cast<unsigned short>(deadband          [0]);
    native.condition.deadband   [1] = static_cast<unsigned short>(deadband          [1]);
    native.condition.deadband   [2] = static_cast<unsigned short>(deadband          [2]);
    native.condition.center     [0] = static_cast<unsigned short>(center            [0]);
    native.condition.center     [1] = static_cast<unsigned short>(center            [1]);
    native.condition.center     [2] = static_cast<unsigned short>(center            [2]);
    return native;
  }
  
  type                       type               = type::spring;
  std::size_t                length             = 1000;
  std::size_t                delay              = 0   ;
  std::size_t                button             = 0   ;
  std::size_t                button_interval    = 0   ;
  std::array<std::size_t, 3> level_right        = {1000, 1000, 1000};
  std::array<std::size_t, 3> level_left         = {1000, 1000, 1000};
  std::array<std::size_t, 3> acceleration_right = {100 , 100 , 100 };
  std::array<std::size_t, 3> acceleration_left  = {100 , 100 , 100 };
  std::array<std::size_t, 3> deadband           = {0   , 0   , 0   };
  std::array<std::size_t, 3> center             = {0   , 0   , 0   };
};
struct haptic_effect_description_constant : haptic_effect_description
{
  SDL_HapticEffect native() const override
  {
    SDL_HapticEffect native;
    native.constant.type             = SDL_HAPTIC_CONSTANT;
    native.constant.direction.type   = SDL_HAPTIC_CARTESIAN;
    native.constant.direction.dir[0] = static_cast<int>           (direction[0]   );
    native.constant.direction.dir[1] = static_cast<int>           (direction[1]   );
    native.constant.direction.dir[2] = static_cast<int>           (direction[2]   );
    native.constant.length           = static_cast<unsigned int>  (length         );
    native.constant.delay            = static_cast<unsigned short>(delay          );
    native.constant.button           = static_cast<unsigned short>(button         );
    native.constant.interval         = static_cast<unsigned short>(button_interval);
    native.constant.level            = static_cast<short>         (level          );
    native.constant.attack_length    = static_cast<unsigned short>(attack_length  );
    native.constant.attack_level     = static_cast<unsigned short>(attack_level   );
    native.constant.fade_length      = static_cast<unsigned short>(fade_length    );
    native.constant.fade_level       = static_cast<unsigned short>(fade_level     );
    return native;
  }

  std::array<std::size_t, 3> direction       = {0, 1, 0};
  std::size_t                length          = 1000;
  std::size_t                delay           = 0   ;
  std::size_t                button          = 0   ;
  std::size_t                button_interval = 0   ;
  std::size_t                level           = 1000;
  std::size_t                attack_length   = 0   ;
  std::size_t                attack_level    = 1000;
  std::size_t                fade_length     = 0   ;
  std::size_t                fade_level      = 1000;
};
struct haptic_effect_description_custom : haptic_effect_description
{
  SDL_HapticEffect native() const override
  {
    SDL_HapticEffect native;
    native.custom.type           = SDL_HAPTIC_CUSTOM;
    native.custom.direction.type = SDL_HAPTIC_CARTESIAN;
    native.custom.direction.dir[0] = static_cast<int>            (direction[0]            );
    native.custom.direction.dir[1] = static_cast<int>            (direction[1]            );
    native.custom.direction.dir[2] = static_cast<int>            (direction[2]            );
    native.custom.length           = static_cast<unsigned int>   (length                  );
    native.custom.delay            = static_cast<unsigned short> (delay                   );
    native.custom.button           = static_cast<unsigned short> (button                  );
    native.custom.interval         = static_cast<unsigned short> (button_interval         );
    native.custom.channels         = 3u;                         
    native.custom.period           = static_cast<unsigned short> (period                  );
    native.custom.samples          = static_cast<unsigned short> (samples.size()          );
    native.custom.data             = const_cast <unsigned short*>(samples.data()[0].data());
    native.custom.attack_length    = static_cast<unsigned short> (attack_length           );
    native.custom.attack_level     = static_cast<unsigned short> (attack_level            );
    native.custom.fade_length      = static_cast<unsigned short> (fade_length             );
    native.custom.fade_level       = static_cast<unsigned short> (fade_level              );
    return native;
  }
  
  std::array<std::size_t, 3>                 direction       = {0, 1, 0};
  std::size_t                                length          = 1000;
  std::size_t                                delay           = 0   ;
  std::size_t                                button          = 0   ;
  std::size_t                                button_interval = 0   ;
  std::size_t                                period          = 1000;
  std::vector<std::array<unsigned short, 3>> samples         = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  std::size_t                                attack_length   = 0   ;
  std::size_t                                attack_level    = 1000;
  std::size_t                                fade_length     = 0   ;
  std::size_t                                fade_level      = 1000;
};
struct haptic_effect_description_left_right : haptic_effect_description
{
  SDL_HapticEffect native() const override
  {
    SDL_HapticEffect native;
    native.leftright.type            = SDL_HAPTIC_LEFTRIGHT;
    native.leftright.length          = static_cast<unsigned int>  (length         );
    native.leftright.large_magnitude = static_cast<unsigned short>(large_magnitude);
    native.leftright.small_magnitude = static_cast<unsigned short>(small_magnitude);
    return native;
  }

  std::size_t length          = 1000;
  std::size_t large_magnitude = 1000;
  std::size_t small_magnitude = 1000;
};
struct haptic_effect_description_periodic : haptic_effect_description
{
  enum class type
  {
    sine          = SDL_HAPTIC_SINE        ,
    triangle      = SDL_HAPTIC_TRIANGLE    ,
    sawtooth_up   = SDL_HAPTIC_SAWTOOTHUP  ,
    sawtooth_down = SDL_HAPTIC_SAWTOOTHDOWN
  };

  SDL_HapticEffect native() const override
  {
    SDL_HapticEffect native;
    native.periodic.type             = static_cast<unsigned short>(type           );
    native.periodic.direction.type   = SDL_HAPTIC_CARTESIAN;
    native.periodic.direction.dir[0] = static_cast<int>           (direction[0]   );
    native.periodic.direction.dir[1] = static_cast<int>           (direction[1]   );
    native.periodic.direction.dir[2] = static_cast<int>           (direction[2]   );
    native.periodic.length           = static_cast<unsigned int>  (length         );
    native.periodic.delay            = static_cast<unsigned short>(delay          );
    native.periodic.button           = static_cast<unsigned short>(button         );
    native.periodic.interval         = static_cast<unsigned short>(button_interval);
    native.periodic.period           = static_cast<unsigned short>(period         );
    native.periodic.magnitude        = static_cast<short>         (magnitude      );
    native.periodic.offset           = static_cast<short>         (offset         );
    native.periodic.phase            = static_cast<unsigned short>(phase * 100.0F );
    native.periodic.attack_length    = static_cast<unsigned short>(attack_length  );
    native.periodic.attack_level     = static_cast<unsigned short>(attack_level   );
    native.periodic.fade_length      = static_cast<unsigned short>(fade_length    );
    native.periodic.fade_level       = static_cast<unsigned short>(fade_level     );
    return native;
  }

  type                       type            = type::sine;
  std::array<std::size_t, 3> direction       = {0, 1, 0};
  std::size_t                length          = 1000;
  std::size_t                delay           = 0   ;
  std::size_t                button          = 0   ;
  std::size_t                button_interval = 0   ;
  std::size_t                period          = 1000;
  std::size_t                magnitude       = 1000;
  std::size_t                offset          = 0   ;
  float                      phase           = 0.0F;
  std::size_t                attack_length   = 0   ;
  std::size_t                attack_level    = 1000;
  std::size_t                fade_length     = 0   ;
  std::size_t                fade_level      = 1000;
};
struct haptic_effect_description_ramp : haptic_effect_description
{
  SDL_HapticEffect native() const override
  {
    SDL_HapticEffect native;
    native.ramp.type             = SDL_HAPTIC_RAMP;
    native.ramp.direction.type   = SDL_HAPTIC_CARTESIAN;
    native.ramp.direction.dir[0] = static_cast<int>           (direction[0]   );
    native.ramp.direction.dir[1] = static_cast<int>           (direction[1]   );
    native.ramp.direction.dir[2] = static_cast<int>           (direction[2]   );
    native.ramp.length           = static_cast<unsigned int>  (length         );
    native.ramp.delay            = static_cast<unsigned short>(delay          );
    native.ramp.button           = static_cast<unsigned short>(button         );
    native.ramp.interval         = static_cast<unsigned short>(button_interval);
    native.ramp.start            = static_cast<short>         (level_start    );
    native.ramp.end              = static_cast<short>         (level_end      );
    native.ramp.attack_length    = static_cast<unsigned short>(attack_length  );
    native.ramp.attack_level     = static_cast<unsigned short>(attack_level   );
    native.ramp.fade_length      = static_cast<unsigned short>(fade_length    );
    native.ramp.fade_level       = static_cast<unsigned short>(fade_level     );
    return native;
  }
  
  std::array<std::size_t, 3> direction       = {0, 1, 0};
  std::size_t                length          = 1000;
  std::size_t                delay           = 0   ;
  std::size_t                button          = 0   ;
  std::size_t                button_interval = 0   ;
  std::size_t                level_start     = 1000;
  std::size_t                level_end       = 2000;
  std::size_t                attack_length   = 0   ;
  std::size_t                attack_level    = 1000;
  std::size_t                fade_length     = 0   ;
  std::size_t                fade_level      = 1000;
};
}

#endif
