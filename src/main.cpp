#include <iostream>
#include "tic-tac-toe.h"
int main()
{
    /*
    Player_t a=Player_t::Computer;
    std::cout<<a<<'\n';
    */
    //const short computer =1 |1<<2 |1<<5;
    //const short player= 2| 1<<3|1<<6;
    game_state g(0,0,Player_t::Computer,0);
    g.compute_stats();
    std::cout<<g;
    while(g.winner==Player_t::NoWiner)
    {
        std::cout<<"COMPUTER\n";
        g=g.optimal_movement();
        std::cout<<g;
        /*
        std::cout<<"Ingese la fila"<<'\n';
        int f;
        std::cin>>f;
        
        std::cout<<"Ingese la col"<<'\n';
        int c;
        std::cin>>c;
        int pos=f*3+c;
        for (auto & pgs:g.possible_game_states)
        {
            if (pgs.selected_movement==pos)g=pgs;
        }
        std::cout<<g;
        */
    }
    


}
