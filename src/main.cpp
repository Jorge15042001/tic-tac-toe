#include <cstdint>
#include <iostream>
#include <string.h>

#include "tic-tac-toe.h"
const std::uint16_t getPlayerPos(const  game_state& gs)
{
    std::uint16_t r;
    std::uint16_t c;

    std::cout<<"Input row and col: \nRow: ";
    std::cin>>r;
    std::cout<<"Col: ";
    std::cin>>c;
    const  std::uint16_t pos= r+r+r+c;
    std::cout<<"position result <"<<r<<","<<c<<"> pos: "<<pos<<'\n';
    if(/*pos >=0 && pos<9&&*/gs.is_playeble_movement(pos))return pos;
    std::cout<<"Not available movement\n";
    return getPlayerPos(gs);
}
void printHelp(const char * filename)
{
    std::cout<<filename<<" -help\tshow help\n";
    std::cout<<filename<<" -Human\tfirst movement done by human\n";
    std::cout<<filename<<" -Computer\tfirst movement done by Computer\n";


}
int main(const int argc, const char *const* const argv)
{
    
    if (argc!=2){
        std::cerr<<"Only 1 argument must be passed\n";
        printHelp(argv[0]);
        return -1;
    }
    game_state * actual_game= new game_state(0,0,Player_t::Computer,0);
    if (strcmp(argv[1],"-help")==0)
    {
        printHelp(argv[0]);
        return 0;
    }
    else if(strcmp(argv[1],"-Human")==0)
    {
        delete actual_game;
        actual_game= new game_state(0,0,Player_t::Human,0);
    }
    else if(strcmp(argv[1],"-Computer")==0)
    {
            
    }
    else 
    {
        std::cout<<argv[1]<<" Not recognice argument\n";
        printHelp(argv[0]);
        return -1;
    }
    /*
    Player_t a=Player_t::Computer;
    std::cout<<a<<'\n';
    */
    //const short computer =1 |1<<2 |1<<5;
    //const short player= 2| 1<<3|1<<6;
    actual_game->compute_stats();
    //game_state new_game = g.optimal_movement();

    std::cout<<*actual_game;
   // 
    while(actual_game->winner==Player_t::NoWiner)
    {
        if (actual_game->actual_player==Player_t::Computer)
        {
            std::cout<<"COMPUTER\n";
            actual_game=actual_game->optimal_movement();
            std::cout<<*actual_game;
        }
        else
        {
            const std::uint16_t pos=getPlayerPos(*actual_game);
            for (auto & pgs:actual_game->possible_game_states)
            {
                if (pgs.selected_movement==pos)
                {
                    actual_game=&pgs;
                }
            }
            std::cout<<"Human\n";
            std::cout<<*actual_game;
        }
    }

}
