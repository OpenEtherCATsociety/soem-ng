
#include "sii.h"
#include "eeprom.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int ec_sii_get_category (ec_net_t * net, ec_slave_t * slave, uint16_t offset,
                         uint16_t * category, uint16_t * length)
{
   uint8_t data[4];
   int result;

   result = ec_eeprom_read (net, slave, offset, data, sizeof(data));
   if (result < 0)
   {
      return result;
   }

   *category = data[0] | data[1] << 8;
   *length = data[2] | data[3] << 8;
   *length = *length * sizeof(uint16_t);

   offset += sizeof(data);
   return offset;
}

int ec_sii_get_string (ec_net_t * net, ec_slave_t * slave, uint16_t offset,
                       char * string, uint16_t length)
{
   int result;

   result = ec_eeprom_read (net, slave, offset, string, length);
   if (result < 0)
   {
      return result;
   }

   offset += length;
   return offset;
}

int ec_sii_get_general (ec_net_t * net, ec_slave_t * slave, uint16_t offset,
                        ec_general_t * general, uint16_t length)
{
   int result;

   result = ec_eeprom_read (net, slave, offset, general, length);
   if (result < 0)
   {
      return result;
   }

   offset += length;
   return offset;
}

int ec_sii_get_fmmu (ec_net_t * net, ec_slave_t * slave, uint16_t offset,
                     uint8_t * fmmu, uint16_t length)
{
   int result;

   result = ec_eeprom_read (net, slave, offset, fmmu, length);
   if (result < 0)
   {
      return result;
   }

   offset += length;
   return offset;
}

int ec_sii_get_sm (ec_net_t * net, ec_slave_t * slave, uint16_t offset,
                   ec_sm_t * sm, uint16_t length)
{
   int result;

   result = ec_eeprom_read (net, slave, offset, sm, length);
   if (result < 0)
   {
      return result;
   }

   offset += length;
   return offset;
}

int ec_sii_parse (ec_net_t * net, ec_slave_t * slave)
{
#ifndef _MSC_VER
   int offset = 0x80;
   uint16_t category;
   uint16_t length;
   unsigned int i;

   do
   {
      offset = ec_sii_get_category (net, slave, offset,
              &category, &length);
      if (offset < 0)
         return offset;

      printf ("C: %d, L: %d\n", category, length);

      switch (category)
      {
      case 10:
         {
            char strings[length];
            int nstrings, count;
            int ix = 0;
            offset = ec_sii_get_string (net, slave, offset, strings, length);
            nstrings = strings[ix++];
            for (count = 0; count < nstrings; count++)
            {
               int slength = strings[ix++];
               char * s = os_malloc (slength + 1);
               memcpy (s, &strings[ix], slength);
               s[slength] = 0;
               ix += slength;
               printf ("%s (%d)\n", s, slength);
            }
            break;
         }
      case 30:
         {
            ec_general_t general;
            offset = ec_sii_get_general (net, slave, offset, &general, length);
            printf ("CoE: %x\n", general.coeDetails);
            printf ("FoE: %x\n", general.foeDetails);
            printf ("EoE: %x\n", general.eoeDetails);
            break;
         }
      case 40:
         {
            uint8_t fmmu[4];
            memset (fmmu, 0, sizeof(fmmu));
            offset = ec_sii_get_fmmu (net, slave, offset, fmmu, length);
            printf ("FMMU: %d %d %d %d\n", fmmu[0], fmmu[1], fmmu[2], fmmu[3]);
            break;
         }
      case 41:
         {
            for (i = 0; i < length/sizeof(ec_sm_t); i++)
            {
               ec_sm_t sm;
               offset = ec_sii_get_sm (net, slave, offset, &sm, sizeof(ec_sm_t));
               printf ("sm%d.start = %x\n", i, sm.address);
               printf ("sm%d.length = %x\n", i, sm.length);
               printf ("sm%d.control = %x\n", i, sm.control);
               printf ("sm%d.type = %x\n", i, sm.type);
            }

            break;
         }
      default:
         offset += length;
         break;
      }

   } while (category != 0xFFFF);
#endif

   return 0;
}
