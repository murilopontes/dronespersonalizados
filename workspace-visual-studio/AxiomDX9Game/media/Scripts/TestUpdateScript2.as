void update()
{
    Vector3 pos(0,0,0);
    BaseEditor @editor = getUpdateObject();
    Vector3 initialPos = editor.getDerivedPosition() - Vector3(cos(0) * 4.0f, 0, sin(0) * 2.0f);
    float deg = 0;
    float rad = 0.0175f;
    
    while(true)
    {
        deg += getTimeSinceLastUpdate() * 120.0f;

        if(deg > 360)
            deg -= 360;

        float degrad = deg * rad;

        if(deg > 90 && deg < 270)
        {
            pos = Vector3(cos(degrad) * 4.0f, 0, sin(degrad) * 2.0f);
        }
        else
        {
            pos = Vector3(cos(degrad) * 4.0f, 0, sin(degrad) * 2.0f);
        }

        pos = pos + initialPos;

        editor.setDerivedPosition(pos);
        yield();
    }
}
