#include <eosio/eosio.hpp>

using namespace eosio;

CONTRACT address: public contract {
    public:

    using contract::contract;

    ACTION countaction(name user){
        require_auth(get_self());

        counts forCount(get_self(), get_self().value);
        auto itr = forCount.find(user.value); //사용자가 어디있는지 가리킴
        
        if(itr == forCount.end()){//user가 없는 경우
            forCount.emplace(user, [&](auto& row){
            row.user = user;
            row.count = 1;//action을 실행한 횟수
            });
        }
        else {
            forCount.modify(itr, user, [&](auto& row){
               
                row.count ++;
            });
        }
        print("SUCCESS!!!");
    }
    
    ACTION findcount(uint64_t count){
        counts forFindcount(get_self(), get_self().value);
        auto forSecondary = forFindcount.get_index<"bycount"_n>();//forfind랑 다른점_ indexing

        auto itr = forSecondary.find(count);
        if(itr != forSecondary.end()){
            print(itr->user, " ", itr->count);
        }
        else {
            print("Nobody has that count number");
        }
    }

    ACTION eraseall() {
        require_auth(get_self());

        counts forEraseAll(get_self(), get_self().value);
           auto itr = forEraseAll.begin();

           while(itr != forEraseAll.end()) { itr = forEraseAll.erase(itr); } //함수의 특징(erase함수의 특징), 다음 이터레이터를 반환하기 때문
           print("!!!ERASE ALL!!!");

           /*for(auto i = forEraseAll.begin(); i = forEraseAll.end();){
               i = forEraseAll.erase(i);
           } */
    }

    private:
    struct [[eosio::table]] countstruct {
        name user; //count의 주인이 되는 계정
        uint64_t count;

        uint64_t primary_key() const { return user.value; }
        uint64_t by_count() const { return count; }//secondary index 이용
    };

    typedef multi_index<"counttable"_n, countstruct,
    indexed_by<"bycount"_n/*secondary index이름 */, const_mem_fun<countstruct, uint64_t, &countstruct::by_count>>> counts;
    //const_mem_fun: secondary index의 정의를 정의한다.
};