#pragma once
#include "Logger.h"
struct Payment {
    string sig;
    double amt;
    uint16_t id;
};
class PaymentProcessor :
    public Logger
{
public:
    PaymentProcessor(double amt, uint16_t id): amt(amt), id(id) {
        log_name = "PAY_PROC";
        log("Payment Processor Initialzed for id %d with amount $%f",
            id, amt);
    }

    Payment pay(string cc) {
        Payment p = { "NULL", 0.0, 0 };
        if (amt == 0.0) {
			log("Charge is $0, returning");
            return p;
        }
        log("Charging $%.2f on credit card %s", amt, cc.c_str());
        // Simple luhn check
        int len = cc.length();
        int sum = 0;
        if (len < 13 || len > 19) {
			log("Wrong number of digits");
            return p;
        }
        for (int i = len - 1; i >= 0; i--) {
            int d = cc[i] - '0';
            if ((len - 1 - i) % 2)
                d *= 2;

            sum += d / 10;
            sum += d % 10;
        }
        if (sum % 10 != 0) {
			log("Invalid CC number, residue: %d", sum % 10);
            return p;
        }
        log("Payment successful");

        // SSC can verify payment if payment processor provides
        // a certificate with a valid signature
        p.sig = "PAID";
        p.amt = amt;
        p.id = id;
        return p;
    }

private:
    double amt;
    uint16_t id;
};

