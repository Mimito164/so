#include <stdio.h>
#include <stdbool.h>
#include <cspecs/cspec.h>
#include "handlers/colas_estados/colas_handler.h"

context (Colas) {
    initialize_cfg("tests/testing.config");

    describe("Se inicializa y destruye todo") {
        it("Debe inicializar todas las listas y agregarlas al diccionario, teniendo el diccionario un tamanio de 10") {
            initialize_lists();
            should_int((int)len_dict()) be equal to(10);
            destroy_lists();
        } end
    } end

    describe("Agregar elementos") {
        before {
            initialize_lists();
        } end

        after {
            destroy_lists();
            gh_lpm();
        } end

        it("Se agrega un elemento a cada lista distinta") {
            t_pcb* pcb1 = create_pcb();
            t_pcb* pcb2 = create_pcb();
            t_pcb* pcb3 = create_pcb();
            t_pcb* pcb4 = create_pcb();
            add_pcb("NEW", pcb1);
            add_pcb("READY", pcb2);
            add_pcb("BLOCKED", pcb3);
            add_pcb("EXIT", pcb4);
            should_int((int)len_list("NEW")) be equal to(1);
            should_int((int)len_list("READY")) be equal to(1);
            should_int((int)len_list("BLOCKED")) be equal to(1);
            should_int((int)len_list("EXIT")) be equal to(1);
        } end

        it("Se agrega mas de un elemento a una lista") {
            t_pcb* pcb1 = create_pcb();
            t_pcb* pcb2 = create_pcb();
            t_pcb* pcb3 = create_pcb();
            t_pcb* pcb4 = create_pcb();
            add_pcb("NEW", pcb1);
            add_pcb("NEW", pcb2);
            add_pcb("NEW", pcb3);
            add_pcb("NEW", pcb4);
            should_int((int)len_list("NEW")) be equal to(4);
        } end
    } end

    describe("Obtener elementos en NEW") {
        before {
            initialize_lists();
            t_pcb* pcb1 = create_pcb();
            t_pcb* pcb2 = create_pcb();
            t_pcb* pcb3 = create_pcb();
            t_pcb* pcb4 = create_pcb();
            t_pcb* pcb5 = create_pcb();
            add_pcb("NEW", pcb1);
            add_pcb("NEW", pcb2);
            add_pcb("NEW", pcb3);
            add_pcb("NEW", pcb4);
            add_pcb("NEW", pcb5);
        } end

        after {
            destroy_lists();
            gh_lpm();
        } end

        it("Se obtiene el primer elemento de la lista NEW") {
            t_pcb* pcb = get_next_pcb("NEW");
            should_int((int)pcb->pid) be equal to(0);
            should_int((int)len_list("NEW")) be equal to(4);
            destroy_pcb(pcb);
        } end

        it("Se obtiene el elemento de la lista NEW con pid 2") {
            t_pcb* pcb = find_pcb_by_pid("NEW", 2);
            should_int((int)pcb->pid) be equal to(2);
            should_int((int)len_list("NEW")) be equal to(4);
            destroy_pcb(pcb);
        } end
    } end

    describe("Obtener elementos en READY") {
        before {
            initialize_lists();
            t_pcb* pcb1 = create_pcb();
            t_pcb* pcb2 = create_pcb();
            t_pcb* pcb3 = create_pcb();
            t_pcb* pcb4 = create_pcb();
            t_pcb* pcb5 = create_pcb();
            add_pcb("READY", pcb1);
            add_pcb("READY", pcb2);
            add_pcb("READY", pcb3);
            add_pcb("READY", pcb4);
            add_pcb("READY", pcb5);
        } end

        after {
            destroy_lists();
            gh_lpm();
        } end

        it("Se obtiene el primer elemento de la lista READY") {
            t_pcb* pcb = get_next_pcb("READY");
            should_int((int)pcb->pid) be equal to(0);
            should_int((int)len_list("READY")) be equal to(4);
            destroy_pcb(pcb);
        } end

        it("Se obtiene el elemento de la lista READY con pid 2") {
            t_pcb* pcb = find_pcb_by_pid("READY", 2);
            should_int((int)pcb->pid) be equal to(2);
            should_int((int)len_list("READY")) be equal to(4);
            destroy_pcb(pcb);
        } end
    } end

    describe("Obtener elementos en BLOCKED") {
        before {
            initialize_lists();
            t_pcb* pcb1 = create_pcb();
            t_pcb* pcb2 = create_pcb();
            t_pcb* pcb3 = create_pcb();
            t_pcb* pcb4 = create_pcb();
            t_pcb* pcb5 = create_pcb();
            add_pcb("BLOCKED", pcb1);
            add_pcb("BLOCKED", pcb2);
            add_pcb("BLOCKED", pcb3);
            add_pcb("BLOCKED", pcb4);
            add_pcb("BLOCKED", pcb5);
        } end

        after {
            destroy_lists();
            gh_lpm();
        } end

        it("Se obtiene el primer elemento de la lista BLOCKED") {
            t_pcb* pcb = get_next_pcb("BLOCKED");
            should_int((int)pcb->pid) be equal to(0);
            should_int((int)len_list("BLOCKED")) be equal to(4);
            destroy_pcb(pcb);
        } end

        it("Se obtiene el elemento de la lista BLOCKED con pid 2") {
            t_pcb* pcb = find_pcb_by_pid("BLOCKED", 2);
            should_int((int)pcb->pid) be equal to(2);
            should_int((int)len_list("BLOCKED")) be equal to(4);
            destroy_pcb(pcb);
        } end
    } end

    describe("Obtener elementos en EXIT") {
        before {
            initialize_lists();
            t_pcb* pcb1 = create_pcb();
            t_pcb* pcb2 = create_pcb();
            t_pcb* pcb3 = create_pcb();
            t_pcb* pcb4 = create_pcb();
            t_pcb* pcb5 = create_pcb();
            add_pcb("EXIT", pcb1);
            add_pcb("EXIT", pcb2);
            add_pcb("EXIT", pcb3);
            add_pcb("EXIT", pcb4);
            add_pcb("EXIT", pcb5);
        } end

        after {
            destroy_lists();
            gh_lpm();
        } end

        it("Se obtiene el primer elemento de la lista EXIT") {
            t_pcb* pcb = get_next_pcb("EXIT");
            should_int((int)pcb->pid) be equal to(0);
            should_int((int)len_list("EXIT")) be equal to(4);
            destroy_pcb(pcb);
        } end

        it("Se obtiene el elemento de la lista EXIT con pid 2") {
            t_pcb* pcb = find_pcb_by_pid("EXIT", 2);
            should_int((int)pcb->pid) be equal to(2);
            should_int((int)len_list("EXIT")) be equal to(4);
            destroy_pcb(pcb);
        } end
    } end

destroy_config_kernel();
destroy_logger();

}