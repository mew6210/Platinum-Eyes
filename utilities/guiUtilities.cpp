#include "../warframe_tool.h"



HRESULT setTransparency(HWND hWnd)
{
    

    HRESULT hr = S_OK;

    
    DWM_BLURBEHIND bb = { 0 };

    
    bb.dwFlags = DWM_BB_ENABLE;
    bb.fEnable = true;
    bb.hRgnBlur = NULL;


    hr = DwmEnableBlurBehindWindow(hWnd, &bb);
    return hr;
}



std::map<int, std::string> createIntStringMap(std::map<std::string, ProductPricing> items) {

    std::map<int, std::string> intstringmap;

    int it = 0;
    for (auto& p : items) {
        intstringmap.insert(std::pair<int,std::string>(it,p.first));
        it++;
    }

    return intstringmap;
}



void generateImGuiTable(std::map<std::string, ProductPricing>& items) {

    if (items.size() != 0) {



        std::map<int, std::string> indexedMap = createIntStringMap(items);

       
        if (ImGui::BeginTable("table1", items.size()))
        {
            for (int row = 0; row < 4; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 4; column++)
                {

                    std::string itemName = indexedMap[column];
                    ProductPricing pricing = items[itemName];
                    const float TEXT_BASE_WIDTH = ImGui::CalcTextSize(itemName.c_str()).x;

                    sf::FloatRect rect;
                    rect.height = 200;
                    rect.width = TEXT_BASE_WIDTH+20;


                    ImGui::TableSetColumnIndex(column);

                    switch (row) {
                    case 0: ImGui::DrawRectFilled(rect, sf::Color(78, 96, 99, 255), 20, ImDrawFlags_RoundCornersAll); break;
                        
                    case 1: ImGui::TextWrapped(itemName.c_str()); break;
                    case 2: ImGui::TextWrapped(std::to_string(pricing.averagePrice).c_str()); break;
                    case 3: ImGui::TextWrapped(getFormatedAveragePrices(pricing.lowestPrices).c_str()); break;



                    }



                }
            }
        }


        
        ImGui::EndTable();
        return;
    }
    else {
        return;
    }




}
