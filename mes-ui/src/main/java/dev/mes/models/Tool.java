package dev.mes.models;

import com.google.gson.annotations.SerializedName;

public class Tool {
    @SerializedName("TOOL_ID")
    private String TOOL_ID;

    @SerializedName("TOOL_NM")
    private String TOOL_NM;

    @SerializedName("WORK_AREA")
    private String WORK_AREA;

    @SerializedName("STATUS")       
    private String STATUS;

    @SerializedName("OPER_TIME")    
    private String OPER_TIME;

    @SerializedName("EMPL_ID")      
    private String EMPL_ID;

    @SerializedName("CHAMBER_CNT")  
    private String CHAMBER_CNT;

    public String getToolID() { return TOOL_ID; }
    public void setToolID(String TOOL_ID) { this.TOOL_ID = TOOL_ID; }
    public String getToolNM() { return TOOL_NM; }
    public void setToolNM(String TOOL_NM) { this.TOOL_NM = TOOL_NM; }
    public String getWorkArea() { return WORK_AREA; }
    public void setWorkArea(String WORK_AREA) { this.WORK_AREA = WORK_AREA; }
    public String getStatus() { return STATUS; }
    public void setStatus(String STATUS) { this.STATUS = STATUS; }
    public String getOperTime() { return OPER_TIME; }
    public void setOperTime(String OPER_TIME) { this.OPER_TIME = OPER_TIME; }
    public String getEmplID() { return EMPL_ID; }
    public void setEmplID(String EMPL_ID) { this.EMPL_ID = EMPL_ID; }
    public String getChamberCnt() { return CHAMBER_CNT; }
    public void setChamberCnt(String CHAMBER_CNT) { this.CHAMBER_CNT = CHAMBER_CNT; }
}