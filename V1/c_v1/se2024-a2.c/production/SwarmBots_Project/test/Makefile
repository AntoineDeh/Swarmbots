#
# SwarmBots - Makefile des tests du projet.
#
# @author Matthias Brun
# @author Joshua MONTREUIL : adaptation du makefile pour l'equipe ProSE A2.
#

#
# Organisation des sources de test.
#

# Packages.
PACKAGES += alphabot2
PACKAGES += controller
PACKAGES += lib
PACKAGES += logs
PACKAGES += com

# Wrap des méthodes du Stock pour bouchonage.

#CONTROLLER_CORE_test :
LDWRAP += -Wl,--wrap=CONTROLLER_CORE_mq_send -Wl,--wrap=CONTROLLER_CORE_set_operating_mode
LDWRAP += -Wl,--wrap=CONTROLLER_CORE_apply_edit_mode -Wl,--wrap=CONTROLLER_CORE_disconnect_core
LDWRAP += -Wl,--wrap=CONTROLLER_CORE_init_hardware
LDWRAP += -Wl,--wrap=SERVO_MOTOR_set_position -Wl,--wrap=SERVO_MOTOR_disable_servo_motor
LDWRAP += -Wl,--wrap=CAMERA_disable_camera -Wl,--wrap=CAMERA_enable_camera -Wl,--wrap=CAMERA_disconnect_camera
LDWRAP += -Wl,--wrap=STATE_INDICATOR_disable_buzzer -Wl,--wrap=STATE_INDICATOR_enable_buzzer
LDWRAP += -Wl,--wrap=STATE_INDICATOR_disable_led -Wl,--wrap=STATE_INDICATOR_enable_led
LDWRAP += -Wl,--wrap=STATE_INDICATOR_set_state -Wl,--wrap=POSTMAN_disconnect
LDWRAP += -Wl,--wrap=PILOT_ask_cmd -Wl,--wrap=CONTROLLER_LOGGER_log -Wl,--wrap=CONTROLLER_RINGER_init_failed_pings_var
LDWRAP += -Wl,--wrap=GUI_SECRETARY_PROXY_set_mode -Wl,--wrap=GUI_SECRETARY_PROXY_ack_connection -Wl,--wrap=GUI_SECRETARY_PROXY_disconnected_ok
LDWRAP += -Wl,--wrap=CONTROLLER_CORE_ask_to_disconnect -Wl,--wrap=CONTROLLER_CORE_ask_set_mode -Wl,--wrap=CONTROLLER_CORE_ask_mode -Wl,--wrap=CONTROLLER_CORE_ask_set_state
LDWRAP += -Wl,--wrap=CAMERA_set_up_ihm_info -Wl,--wrap=CONTROLLER_LOGGER_logs_saved -Wl,--wrap=CONTROLLER_LOGGER_ask_set_rtc -Wl,--wrap=CONTROLLER_LOGGER_ask_logs
LDWRAP += -Wl,--wrap=CONTROLLER_RINGER_ask_availability -Wl,--wrap=POSTMAN_read_request -Wl,--wrap=POSTMAN_send_request
LDWRAP += -Wl,--wrap=DISPATCHER_decode_message -Wl,--wrap=DISPATCHER_dispatch_received_msg
#STATE_INDICATOR_test :
LDWRAP += -Wl,--wrap=STATE_INDICATOR_add_msg_to_queue -Wl,--wrap=STATE_INDICATOR_action_notify_selected
LDWRAP += -Wl,--wrap=LEDS_set_color -Wl,--wrap=LEDS_start_blinking -Wl,--wrap=BUZZER_enable_buzzer -Wl,--wrap=BUZZER_disable_buzzer
LDWRAP += -Wl,--wrap=watchdog_start -Wl,--wrap=watchdog_cancel
LDWRAP += -Wl,--wrap=CONTROLLER_RINGER_add_msg_to_queue -Wl,--wrap=CONTROLLER_RINGER_update_failed_pings -Wl,--wrap=GUI_RINGER_PROXY_set_availability -Wl,--wrap=CONTROLLER_RINGER_can_still_fail_pings
LDWRAP += -Wl,--wrap=CONTROLLER_RINGER_has_too_much_failed_pings -Wl,--wrap=CONTROLLER_CORE_connection_lost -Wl,--wrap=MOTOR_set_velocity -Wl,--wrap=CONTROLLER_CORE_get_mode
LDWRAP += -Wl,--wrap=CONTROLLER_CORE_get_id_robot -Wl,--wrap=RADAR_get_radar -Wl,--wrap=PILOT_action_check_radar -Wl,--wrap=PILOT_add_msg_to_queue
LDWRAP += -Wl,--wrap=GUI_SECRETARY_PROXY_set_radar

#Méthodes bouchonnées pour des besoins internes à un module.
CCFLAGS += -D_WRAP_STATIC_FUNCTIONS_MOCKERY_CMOCKA #Pour les méthodes statiques
CCFLAGS += -D_WRAP_MQ_CONTROLLER_RINGER_MOCKERY_CMOCKA #Pour le mock de la mq de controllerRinger
#CCFLAGS += -D_VERIF_EFFETS_BORDS


# Deux niveaux de packages sont accessibles
SRC  = $(wildcard */*.c)
SRC += $(wildcard */*/*.c)

OBJ = $(SRC:.c=.o)

# Point d'entrée du programme.
MAIN = main_test.c

# Gestion automatique des dépendances.
DEP = $(MAIN:.c=.d)

# Executable à générer.
EXEC = ../$(TEST)

# Inclusion depuis le niveau du package.
CCFLAGS += -I. -I$(CMOCKA)/include

# Liens pour les tests.
LDFLAGS += -L$(CMOCKA)/lib

# Utilisation de la librairie static CMocka
LDFLAGS += -lcmocka-static
# Force l'utilisation d'une librairie statique pour CMocka.
# LDFLAGS += -Wl,-static -lcmocka-static -Wl,-Bdynamic

# Gcov informations (-fprofile-abs-path pour compatibilité avec IDE, nécessite realpath sur obj en compilation)
ifneq ($(TARGET), raspberry)
CCFLAGS += -fprofile-arcs -ftest-coverage -fprofile-abs-path
endif

GCDA = $(MAIN:.c=.gcda)
GCNO = $(MAIN:.c=.gcno)

GCOVR = gcovr
GCOVR_DIR = ../report/

#
# Règles du Makefile.
#

.PHONY: all clean $(PACKAGES)

# Compilation.
all: $(PACKAGES)
	$(MAKE) $(EXEC)

$(EXEC): $(OBJ) $(MAIN)
	$(CC) $(CCFLAGS) $(LDWRAP) $(realpath $(OBJ)) $(MAIN) -MF $(DEP) -o $(EXEC) $(LDFLAGS)
	$(MAKE) clean_coverage

# Nettoyage.
clean: $(PACKAGES) test_report_clean
	@rm -f $(DEP) $(GCNO)

clean_coverage: $(PACKAGES)
	@rm -f $(GCDA)

# Packages
$(PACKAGES):
	$(MAKE) $(MAKECMDGOALS) -C $@

# Gcov
test_report:
	$(GCOVR) -r .. --html --html-details -o $(GCOVR_DIR)/index.html

test_report_clean:
	$(MAKE) clean_coverage
	@rm -f $(GCOVR_DIR)/*.html

	
-include $(DEP)

