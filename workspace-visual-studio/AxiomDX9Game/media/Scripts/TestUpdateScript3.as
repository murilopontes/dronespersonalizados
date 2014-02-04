   
void update()
{
    Vector3 pos(0,0,0);
    BaseEditor @editor = getUpdateObject();
    Vector3 initialPos = editor.getDerivedPosition();
    
    while(true)
    {
        EditorVector list;
        EditorVector list2;
        utils.sphereQuery(editor, 2.0f, list);
        bool sleepmode = false;

        root.regexpByName('ball.*',false, list, list2);

        if(list2.size() > 0)
        {
            pos.y += (getTimeSinceLastFrame() * 5.0f);
        
            if(pos.y > 1.0f)
            {
               pos.y = 1.0f;
               sleepmode = true;
            }
        }
        else
        {
            pos.y -= (getTimeSinceLastFrame() * 2.6f);

            if(pos.y < 0)
                pos.y = 0;
        }

        Vector3 newpos = pos + initialPos;
    
        editor.setDerivedPosition(newpos);
        
        if(sleepmode)
            sleep(500);
        else
            yield();
    }
}
