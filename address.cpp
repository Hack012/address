#include <eosio/eosio.hpp>

using namespace eosio;

CONTRACT address: public contract {
    public:

    using contract::contract;

    ACTION countuser(name user){
        require_auth(get_self());

        address_index forCountuser(get_self(), get_self().value);
        auto itr = forCountuser.require_find(user.value, "NO USER, INSERT IT");
        if(itr != forCountuser.end()){
            forCountuser.modify(itr, user, [&](auto& row){
                row.user = user;
            row.count ++;
            });
        }
        else {
            forCountuser.emplace(user, [&](auto& row){
                row.user = user;
                row.count ++;
            });
        }
        print(itr -> count);
    }
    
    ACTION findcount(uint64_t count){
        address_index forFindcount(get_self(), get_self().value);
        auto forSecondary = forFindcount.get_index<"bycount"_n>();

        auto itr = forFindcount.require_find(count, "NO");

        print (itr->user);
    }

    ACTION insert(name user){
        require_auth(get_self());


        address_index forInsert(get_self(), get_self().value);
        auto itr = forInsert.find(user.value);

        check(itr == forInsert.end(), "already exists");

        forInsert.emplace(user, [&](auto& row){
            row.user = user;
        });

        print("!!!INSERT SUCCESS!!!");
    }

    ACTION eraseall() {
        require_auth(get_self());

        address_index forEraseAll(get_self(), get_self().value);
           auto itr = forEraseAll.begin();
           while(itr != forEraseAll.end()) { itr = forEraseAll.erase(itr); }
           itr ++;

           print("!!!ERASE ALL!!!");
    }

    private:
    struct [[eosio::table]] person {
        name user;
        uint64_t count;

        uint64_t primary_key() const { return user.value; }
        uint64_t by_count() const { return count; }
    };

    typedef multi_index<"counttable"_n, person,
    indexed_by<"bycount"_n, const_mem_fun<person, uint64_t, &person::by_count>>> address_index;
};