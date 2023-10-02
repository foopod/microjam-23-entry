#include "foopod_get_familiar_game.h"

#include "bn_keypad.h"
#include "bn_log.h"
#include "bn_math.h"

#include "mj/mj_game_list.h"

MJ_GAME_LIST_ADD(foopod::get_familiar_game)

namespace foopod
{

    get_familiar_game::get_familiar_game(int completed_games, const mj::game_data &data)
    {
        _left.set_visible(false);
        _right.set_visible(false);
        // first in the array is always the correct answer
        _option1.push_back(bn::sprite_items::foopod_get_familiar_yarn.create_sprite(0,0));
        _option1.push_back(bn::sprite_items::foopod_get_familiar_bowl.create_sprite(0,0));

        for(bn::sprite_ptr option : _option1){
            option.set_visible(false);
        }

        constexpr int frames_diff = maximum_frames - minimum_frames;
        constexpr int maximum_speed_completed_games = 30;

        completed_games = bn::min(completed_games, maximum_speed_completed_games);

        int frames_reduction = (frames_diff * completed_games) / maximum_speed_completed_games;
        _total_frames = maximum_frames - frames_reduction;
        _total_frames -= data.random.get_int(60);
        _total_frames = bn::clamp(_total_frames, minimum_frames, maximum_frames);
    }

    void get_familiar_game::fade_in([[maybe_unused]] const mj::game_data &data)
    {
    }

    mj::game_result get_familiar_game::play(const mj::game_data &data)
    {
        mj::game_result result;
        result.exit = data.pending_frames == 0;
        data.random.update();

        if(_state == STATE::POPPING_IN){
            int rng = data.random.get_int(2);
            BN_LOG(rng);
            if(rng){
                _left = _option1[0];
                _right = _option1[1];
            } else {
                _left = _option1[1];
                _right = _option1[0];
                _correct_is_left = false;
            }
            _left.set_position(bn::fixed_point(-64, 0));
            _right.set_position(bn::fixed_point(64, 0));
            
            for(bn::sprite_ptr option : _option1){
                option.set_visible(true);
            }
            _state = STATE::CHOOSING;
        } else if(_state == STATE::CHOOSING){
            if (bn::keypad::left_pressed())
            {
                // choose left
                if(_correct_is_left){
                    // correct
                } else {
                    // incorrect
                }
            }

            if (bn::keypad::right_pressed())
            {
                // choose right
                if(_correct_is_left){
                    // incorrect
                } else {
                    // correct
                }
            }
        } else if(_state == STATE::SHOWING_RESULT){

        }



        return result;
    }

    void get_familiar_game::fade_out([[maybe_unused]] const mj::game_data &data)
    {
    }
}
