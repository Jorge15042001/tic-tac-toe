//engine for playing tic-tac-toe
//matrix game layout
//0 1 2
//
//0 1 1
//2 1 2
//0 - reprensents empty space
//1 - reprensents player 
//2 -reprensents computer
//
//this game can be represented as 2 integers 
//payer1 = 0b010011010
//payer2 = 0b001000101
#include <bitset>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>
#include <array>
#include <algorithm>
template<typename T>
std::bitset<sizeof(T)*8> get_bit_representation(const T n)
{
    std::bitset<sizeof(T)*8>x(n);
    return x;
}
enum class Player_t
{
    NoWiner=0,
    Computer=1,
    Human=2
};
std::ostream& operator<<(std::ostream& o, const Player_t &value)
{
    if(value==Player_t::NoWiner)o<<"NoWiner";
    else if(value==Player_t::Computer)o<<"Computer";
    else if(value==Player_t::Human)o<<"Human";
    /*
    switch (value) {
        case Player_t::Computer:o<<"Computer\n";break;
        case Player_t::Human:o<<"Human\n";break;
        case Player_t::NoWiner:o<<"NoWiner\n";break;
    }
    */
    return o;
}
struct game_state
{
    const std::uint16_t computer;
    const std::uint16_t player;
    const Player_t actual_player;
    Player_t  winner;
    const std::uint8_t selected_movement;
    std::vector<game_state> possible_game_states;
    long win_computer=0;
    long win_human=0;
    long tie=0;

    game_state operator =( game_state other)
    {
        return other;

    }

    game_state( const std::uint16_t computer,
                const std::uint16_t player, 
                const Player_t next_movement,
                const  std::uint8_t selected_movement):
        computer(computer),
        player(player),
        actual_player(next_movement),
        selected_movement(selected_movement)
    {
        possible_game_states.reserve(9);
        winner=compute_winer(); 
        expand_posibility_tree();
    }

    const Player_t compute_winer(){
        static const std::uint16_t mask_all_row=1|2|4;//0b0000000111 
        static const std::uint16_t mask_all_col=1|1<<3|1<<6;//0b000001001001
        static const std::uint16_t mask_diagonal1=1|1<<4|1<<8;
        static const std::uint16_t mask_diagonal2=1<<2|1<<4|1<<6;
        for (int i=0;i<3;i++)//check allrows and cols
        {
            const std::uint16_t mask_row_shifted=mask_all_row<<(3*i);
            const std::uint16_t mask_col_shifted=mask_all_col<<i;

            if((computer&mask_row_shifted)==mask_row_shifted)return Player_t::Computer;
            if((computer&mask_col_shifted)==mask_col_shifted)return Player_t::Computer;
            if((player&mask_row_shifted)==mask_row_shifted)return Player_t::Human;
            if((player&mask_col_shifted)==mask_col_shifted)return Player_t::Human;
        }
        //check diagonals 
        if((computer&mask_diagonal1)==mask_diagonal1)return Player_t::Computer;
        if((computer&mask_diagonal2)==mask_diagonal2)return Player_t::Computer;
        if((player&mask_diagonal1)==mask_diagonal1)return Player_t::Human;
        if((player&mask_diagonal2)==mask_diagonal2)return Player_t::Human;
        
        return Player_t::NoWiner;

    }
    void compute_stats()
    {
        compute_stats(*this);
    }
    void compute_stats(game_state& gs)
    {
        if(gs.possible_game_states.empty())
        {
            if(gs.winner==Player_t::Computer)gs.win_computer++;
            else if(gs.winner==Player_t::Human)gs.win_human++;
            else gs.tie++;
            return;
        }
        
        for(auto & pgs:gs.possible_game_states)
        {
            compute_stats(pgs);
        }
        for(auto& pgs:gs.possible_game_states)
        {
            gs.win_computer+=pgs.win_computer;
            gs.win_human+=pgs.win_human;
            gs.tie+=pgs.tie;
        }
        
    }

    bool is_playeble_movement(const std::uint8_t  pos) const
    {
       std::uint16_t  mask=1<<pos;
       return (~computer&mask)&(~player&mask);//if not of the bits at pos are set
    }
    
    
    void expand_posibility_tree() 
    {
        //get whose turn is it 
        for (int i=0;i<9;i++)
        {
            if(is_playeble_movement(i) && winner==Player_t::NoWiner)
            {
                //compute the next movement state if  i is the selected pos
                std::uint16_t mask= 1<<i;
                std::uint16_t new_computer_state=computer;
                std::uint16_t new_player_state=player;
                Player_t next_player=Player_t::Computer;
                if(actual_player==Player_t::Computer)//if computer was the last player
                {
                    new_computer_state|=mask;
                    next_player=Player_t::Human;///set player as human
                }
                else if(actual_player==Player_t::Human)
                {
                    new_player_state|=mask;
                }
                
                possible_game_states.emplace_back(
                        new_computer_state,
                        new_player_state,
                        next_player,
                        i);
            }
        }
        
    }
    bool is_set_bit(const std::uint16_t value,const std::uint8_t bit_n) const 
    {
        return  value&(1<<bit_n);
    }

    Player_t get_player_movement(std::uint8_t n) const 
    {
        if(is_set_bit(player,n))return Player_t::Human;
        else if(is_set_bit(computer,n))return Player_t::Computer;
        return Player_t::NoWiner;
    }
    friend std::ostream & operator<<(std::ostream& o, const game_state& value );
    void overload_as_matrix(std::ostream & o) const
    {
        
        o<<"\n-------\n";
        for (std::uint8_t row=0;row<3;row++)
        {
            o<<'|';
            for (std::uint8_t col=0;col<3;col++)
            {
                const Player_t p=get_player_movement(row*3+col);
                if(p==Player_t::Computer)o<<'#';
                else if(p==Player_t::Human) o<<'@';
                else o<<' ';
                //o<<value.get_player_movement(row*3+col)<<'\t';
                o<<"|";
            }
            o<<"\n-------\n";
        }
        //o<<"game stats\nWins: "<<win_computer<<"\nLoss: "<<win_human<<"\nTies: "<<tie<<'\n';
        //o<<"Score: "<<win_computer-win_human<<'\n';
        o<<"WINER: "<<winner<<"\n";
        //o<<"Actual Player: "<<actual_player<<'\n';


    }
    game_state* optimal_movement()
    {
        return &*std::max_element(
                possible_game_states.begin(),
                possible_game_states.end(),
                [](const auto& a,const auto & b){return a.get_score()<b.get_score();}
                );
    }
    const double get_score()const 
    {
        return get_score(*this,1);
        
    }
    const double get_score(const game_state&gs,const int level)const
    {
        if(gs.possible_game_states.empty())return gs.win_computer/level-gs.win_human;
        double score=0;
        for (const auto & pgs:gs.possible_game_states)
        {
            score+=get_score(pgs,level+1);
        }
        return score;
    }
};

std::ostream & operator<<(std::ostream& o, const game_state& value )
{
    value.overload_as_matrix(o);
//    for (const auto & pgs:value.possible_game_states)pgs.overload_as_matrix(o);
    return o<<"\n\n";
}
