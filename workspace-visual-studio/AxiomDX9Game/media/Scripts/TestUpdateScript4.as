   
void update()
{
    Vector3 pos(0,0,0);
    BaseEditor @editor = getUpdateObject();
    Vector3 initialPos = editor.getDerivedPosition();
    Vector3 initScale(0.1f, 0.1f, 0.1f);
    
    for(int i = 0; i < 10;i++)
    {
        pos.x = rand(initialPos.x - 5.0f, initialPos.x + 5.0f);
        pos.z = rand(initialPos.z - 5.0f, initialPos.z + 5.0f);
        pos.y = root.getTerrainEditor().getHeightAt(pos.x, pos.z) + 0.45f;    

        EntityEditor @obj = root.createEntityObject(null, 'crate.mesh');
        if(@obj !is null)
        {
            obj.setDerivedPosition(pos);
            obj.setDerivedScale(initScale);
            obj.load(true);
            obj.setUpdateScript('TestUpdateScript3.as');
        }
        yield();
    }

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
