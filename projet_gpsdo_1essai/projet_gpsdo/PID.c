//#include "USB.h"
#include "PID.h"
#include <avr/io.h>
//#include "Descriptors.h"
#include <stdint.h>
#include "VirtualSerial.h"

float PID(float consigne, float freq_mesure) {
    static const float Kp = 1.2f;   // Gain proportionnel
    static const float Ki = 1.2f;   // Gain intégral
    static const float Kd = 0.01f;  // Gain dérivé

    static float integrale = 0.0f;
    static float erreur_avant = 0.0f;
    static float dt =1.0f;

    // Calcul de l'erreur
    float erreur = freq_mesure;
    integrale += erreur * dt;
    sature_integrale(&integrale);
    float derivee = (erreur - erreur_avant) / dt;
    //float commande_pwm = Kp * (erreur-erreur_avant) + Ki * erreur +consigne;//integrale + Kd * derivee;
    float commande_pwm = Kp * erreur + Ki * integrale + Kd * derivee;
    commande_pwm = sature_commande(commande_pwm);
    erreur_avant = erreur;
    return commande_pwm;
}

// Fonction limite l'intégrale
void sature_integrale(float *integrale) {
    const float integrale_max = 1000.0f;
    if (*integrale > integrale_max) *integrale = integrale_max;
    else if (*integrale < -integrale_max) *integrale = -integrale_max;
}

// Fonction de saturation de la commande PWM
float sature_commande(float commande) {
    if (commande > 50.0f) return 50.0f; //limite de 1023 (8bits)
    if (commande < -50.0f) return -50.0f;
    return commande;
}
