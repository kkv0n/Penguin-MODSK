const char spec_mode[] = "SPECTATING...";

#ifndef REBUILD_PC
#ifdef USE_ONLINE
void StatsUpgrade()
{
    // Verifica si el tipo de motor es 0
    if (octr->enginetype == 0)
    {
        /*
            Stat 9 is acceleration,
            Stats 11 and 12 are speed related
        */
        for (int i = 9; i < 13; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                data.metaPhys[i].value[j] = data.metaPhys[i].value[4]; // copia MAX
            }
        }
    }
    else
    {
        // Si el tipo de motor no es 0
        for (int i = 0; i < 65; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                int engineValue = (octr->enginetype == 5) ? 0 : octr->enginetype;
                data.metaPhys[i].value[j] = data.metaPhys[i].value[engineValue];
            }
        }
    }
}

#endif
#endif