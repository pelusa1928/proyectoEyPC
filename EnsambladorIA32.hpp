#ifndef ENSAMBLADOR_IA32_HPP
#define ENSAMBLADOR_IA32_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <cstdint>

using namespace std;

// --- ESTRUCTURAS DE DATOS ---
struct ReferenciaPendiente {
    int posicion;          // Posición en codigo_hex donde va el parche
    int tamano_inmediato;  // 1 o 4 (byte o dword)
    int tipo_salto;        // 0 = absoluto, 1 = relativo
};

class EnsambladorIA32 {
public:
    // Constructor
    EnsambladorIA32();

    // Ensamblado en dos pasadas
    void ensamblar(const string& archivo_entrada);

    // Resolver referencias (relativas/absolutas)
    void resolver_referencias_pendientes();

    // Generar código máquina en hexadecimal
    void generar_hex(const string& archivo_salida);

    // Generar reportes de tablas
    void generar_reportes();

private:
    // --- ESTADO DEL ENSAMBLADOR ---
    int contador_posicion;           // Location Counter
    bool primera_pasada;             // true = 1ª pasada, false = 2ª pasada
    vector<string> lineas_fuente;    // Líneas crudas del programa.asm

    // Tablas de ensamblado
    unordered_map<string, int> tabla_simbolos; 
    unordered_map<string, vector<ReferenciaPendiente>> referencias_pendientes;
    vector<uint8_t> codigo_hex;     

    // Mapas para codificación de registros
    unordered_map<string, uint8_t> reg32_map; 
    unordered_map<string, uint8_t> reg8_map;  

    // --- FUNCIONES DE SOPORTE ---
    void inicializar_mapas();
    void leer_fuente(const string& archivo);     // Lee archivo a lineas_fuente
    void limpiar_linea(string& linea);
    bool es_etiqueta(const string& s);
    void procesar_etiqueta(const string& etiqueta_cruda);
    void procesar_linea(string linea);
    void procesar_instruccion(const string& linea);

    // Separar "dest, src"
    bool separar_operandos(const string& linea_operandos,
                           string& dest_str,
                           string& src_str);

    // [LABEL] simple (sin +, sin registros)
    bool is_mem_simple_label(const string& op);

    // --- UTILIDADES DE CODIFICACIÓN ---
    uint8_t generar_modrm(uint8_t mod, uint8_t reg, uint8_t rm);
    void agregar_byte(uint8_t byte);
    void agregar_dword(uint32_t dword);

    bool obtener_reg32(const string& op, uint8_t& reg_code);
    bool obtener_reg8(const string& op, uint8_t& reg_code);
    bool obtener_inmediato32(const string& str, uint32_t& immediate);

    // Direccionamientos de memoria
    bool procesar_mem_simple(const string& operando,
                             uint8_t& modrm_byte,
                             const uint8_t reg_code,
                             bool es_destino,
                             uint8_t op_extension = 0);

    bool procesar_mem_sib(const string& operando,
                          uint8_t& modrm_byte,
                          const uint8_t reg_code,
                          bool es_destino);

    bool procesar_mem_disp(const string& operando,
                           uint8_t& modrm_byte,
                           const uint8_t reg_code,
                           bool es_destino);

    // --- FUNCIONES DE PROCESAMIENTO DE INSTRUCCIONES ---
    void procesar_mov(const string& operandos);
    void procesar_add(const string& operandos);
    void procesar_sub(const string& operandos);
    void procesar_cmp(const string& operandos);
    void procesar_imul(const string& operandos);
    void procesar_inc(const string& operandos);
    void procesar_dec(const string& operandos);
    void procesar_mul(const string& operandos);
    void procesar_div(const string& operandos);
    void procesar_idiv(const string& operandos);
    void procesar_xor(const string& operandos);
    void procesar_and(const string& operandos);
    void procesar_or(const string& operandos);
    void procesar_test(const string& operandos);
    void procesar_movzx(const string& operandos);
    void procesar_xchg(const string& operandos);
    void procesar_lea(const string& operandos);
    void procesar_call(const string& operandos);
    void procesar_loop(const string& operandos);
    void procesar_push(const string& operandos);
    void procesar_pop(const string& operandos);
    void procesar_leave();
    void procesar_ret();
    void procesar_nop();
    void procesar_jmp(const string& operandos);
    void procesar_condicional(const string& mnem, const string& operandos);

    // Binaria genérica: ADD / SUB / CMP / AND / OR / XOR
    void procesar_binaria(const string& mnem,
                          const string& operandos,
                          uint8_t opcode_rm_reg,
                          uint8_t opcode_reg_rm,
                          uint8_t opcode_eax_imm,
                          uint8_t opcode_imm_general,
                          uint8_t reg_field_extension);
};

#endif // ENSAMBLADOR_IA32_HPP




