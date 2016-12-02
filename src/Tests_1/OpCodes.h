#ifndef OPCODESMM_H
#define OPCODESMM_H

#include "typeDefs.h"
#include "opcodeEnum.h"

class connection; 

#ifndef OP_FUNCTION
typedef bool(WINAPI *op_function)(connection *, void* argv[]);
#endif

struct opcode_data
{
	e_opcode opCodeValue;
	op_function opFunction;
	bool hasBody;
};


static opcode_data* opCodesArray[OPCODE_MAX];
static uint32 opCodeCount;

op_function opcode_resolve(e_opcode, bool* hasBody = 0);
static bool opcode_add(e_opcode, op_function, bool hasBody = true);

bool opcode_init();
void opcode_release();

//init
bool WINAPI op_check_version(connection *, void* argv[]);
bool WINAPI op_login_arbiter(connection *, void* argv[]);
bool WINAPI op_set_visible_range(connection *, void* argv[]);
bool WINAPI op_get_user_list(connection *, void* argv[]);
bool WINAPI op_hardware_info(connection *, void* argv[]);
bool WINAPI op_request_vip_system_info(connection *, void* argv[]);
bool WINAPI op_get_user_guild_logo(connection *, void* argv[]);
//lobby
bool WINAPI op_can_create_player(connection *, void* argv[]);
bool WINAPI op_create_player(connection *, void* argv[]);
bool WINAPI op_change_user_slot_id(connection *, void* argv[]);
bool WINAPI op_check_username(connection *, void* argv[]);
bool WINAPI op_str_evaluate_string(connection *, void* argv[]);
bool WINAPI op_select_player(connection *, void* argv[]);
bool WINAPI op_delete_player(connection *, void* argv[]);
bool WINAPI op_cancel_delete_player(connection *, void* argv[]);


//world
bool WINAPI op_load_topo_fin(connection *, void* argv[]);
bool WINAPI op_return_to_lobby(connection *, void* argv[]);
bool WINAPI op_cancel_return_to_lobby(connection *, void* argv[]);
bool WINAPI op_guard_pk_policy(connection *, void* argv[]);
bool WINAPI op_move_player(connection *, void* argv[]);
bool WINAPI op_npc_guild_list(connection *, void* argv[]);
bool WINAPI op_raign_info(connection *, void* argv[]);
bool WINAPI op_request_gamestat_ping(connection *, void* argv[]);
bool WINAPI op_social(connection *, void* argv[]);
bool WINAPI op_visit_new_section(connection *, void* argv[]);
bool WINAPI op_guard_pk_policy(connection *, void* argv[]);

#endif


