#ifndef _FREERTOS_VAR_H
#define _FREERTOS_VAR_H

#define EM_DATA_QUEUE_SIZE 2

typedef struct StructEnergyData
{
    float fKWh_pha, fKVAh_pha, fKVArh_pha;
    float fKWh_phb, fKVAh_phb, fKVArh_phb;
    float fKWh_phc, fKVAh_phc, fKVArh_phc;
} EnergyData_t;

typedef struct StructPowerData
{
    float fKW_pha, fVA_pha, fVAr_pha;
    float fKW_phb, fVA_phb, fVAr_phb;
    float fKW_phc, fVA_phc, fVAr_phc;
} PowerData_t;

typedef struct StructPhaseAngle
{
    float fPhiA, fPhiB, fPhiC;
} PhaseAngle_t;

typedef struct StructEMData
{
    float fVoltage_pha, fVoltage_phb, fVoltage_phc;
    float fCurrent_pha, fCurrent_phb, fCurrent_phc;
    EnergyData_t *pStructEnergyData;
    PowerData_t *pStructPowerData;
    PhaseAngle_t *pStructPhaseAngleData;
    float fPF;
} EMData_t;

EMData_t *pStructEMData;
#endif