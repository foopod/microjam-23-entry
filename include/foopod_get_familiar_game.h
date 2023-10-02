#ifndef FOOPOD_GET_FAMILIAR_GAME_H
#define FOOPOD_GET_FAMILIAR_GAME_H

#include "bn_regular_bg_ptr.h"
#include "bn_sprite_ptr.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_sprite_animate_actions.h"

#include "bn_regular_bg_items_foopod_get_familiar_bg.h"
#include "bn_sprite_items_foopod_get_familiar_bowl.h"
#include "bn_sprite_items_foopod_get_familiar_yarn.h"

#include "mj/mj_game.h"

namespace foopod
{
    enum class STATE
    {
        POPPING_IN = 1, 
        CHOOSING = 2,
        SHOWING_RESULT = 3,

    };

}

namespace foopod
{

    class get_familiar_game : public mj::game
    {

    public:
        get_familiar_game(int completed_games, const mj::game_data &data);

        [[nodiscard]] bn::string<16> title() const final
        {
            return "Get Familiar!";
        }

        [[nodiscard]] int total_frames() const final
        {
            return _total_frames;
        }

        void fade_in(const mj::game_data &data) final;

        [[nodiscard]] mj::game_result play(const mj::game_data &data) final;

        [[nodiscard]] bool victory() const final
        {
            return _victory;
        }

        void fade_out(const mj::game_data &data) final;

    private:
        bn::regular_bg_ptr _bg = bn::regular_bg_items::foopod_get_familiar_bg.create_bg(0, 0);
        bn::vector<bn::sprite_ptr, 2> _option1;
        bn::vector<bn::sprite_ptr, 2> _option2;
        bn::vector<bn::sprite_ptr, 2> _option3;
        bn::vector<bn::sprite_ptr, 2> _option4;
        bn::vector<bn::sprite_ptr, 2> _option5;

        STATE _state = STATE::POPPING_IN;

        bn::sprite_ptr _left = bn::sprite_items::foopod_get_familiar_yarn.create_sprite(0,0);
        bn::sprite_ptr _right = bn::sprite_items::foopod_get_familiar_yarn.create_sprite(0,0);

        int _current_level = 0;
        int _max_level = 0;
        bool _correct_is_left = true;

        int _total_frames;
        int _show_result_frames = 60;
        bool _victory = false;
        bool _defeat = false;
    };

}

#endif
