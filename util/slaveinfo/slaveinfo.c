
#include <ec_api.h>
#include <stdio.h>

static ec_slave_config_t cfg[] =
{
   { 0x00000002, 0x044c2c52, 0x01100000, {{0}}, {{0}} },
   {
      0x00000002, 0x0fa13052, 0x01000000,
      {
         { 0x1000, 0x80, 0x10026 },
         { 0x1080, 0x80, 0x10022 },
         { 0x1100, 0x02, 0x10024 },
         { 0x1180, 0x00, 0x00020 },
      },
      {
         { 0, 2, 0, 7, 0x1100, 0, 1, 0 },
      },
   },
   {
      0x00000002, 0x0bf53052, 0x01000000,
      {
         { 0x1000, 0x80, 0x10026 },
         { 0x1080, 0x80, 0x10022 },
         { 0x1100, 0x00, 0x00004 },
         { 0x1180, 0x02, 0x10020 },
      },
      {
         { 3, 2, 0, 7, 0x1180, 0, 1, 0 },
      },
   },
   { 0x00000002, 0x03f03052, 0x01100000, { { 0x1000, 1, 0x10000 } }, {{0}} },
   { 0x00000002, 0x03f03052, 0x01100000, { { 0x1000, 1, 0x10000 } }, {{0}} },
   { 0x00000002, 0x0a3e3052, 0x01100000, { { 0x0f00, 1, 0x90044 } }, {{0}} },
   { 0x00000002, 0x0a3e3052, 0x01100000, { { 0x0f00, 1, 0x90044 } }, {{0}} },
   { 0x00000002, 0x0a3e3052, 0x01100000, { { 0x0f00, 1, 0x90044 } }, {{0}} },
   { 0x00000002, 0x0a3e3052, 0x01100000, { { 0x0f00, 1, 0x90044 } }, {{0}} },
};

int main (int argc, char * argv[])
{
   ec_net_t * net;
   ec_slave_t * slave;

   if (argc != 2)
   {
      ec_nic_show_adapters();
      return -1;
   }

   net = ec_net_init(argv[1]);

   for (slave = ec_net_get_slave (net, 0);
        slave != NULL;
        slave = ec_slave_get_next (slave))
   {
      printf ("Found 0x%08x:0x%08x:0x%08x  %s\n",
              ec_slave_get_vendor (slave),
              ec_slave_get_product (slave),
              ec_slave_get_revision (slave),
              ec_slave_is_coe (slave) ? "CoE" : ""
              );
   }

   ec_config_static (net, cfg);

#if 0
   ec_eeprom_dump (net, ec_net_get_slave(net, 2));
   ec_sii_parse (net, ec_net_get_slave (net, 2));
#endif

   return 0;
}
