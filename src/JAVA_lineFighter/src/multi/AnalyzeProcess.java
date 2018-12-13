package multi;

public class AnalyzeProcess {
	/**
	 *  ‰»Î≤Œ ˝
	 */
	private String inputStreamString;
	
	public AnalyzeProcess(String inputStreamString){
		this.inputStreamString=inputStreamString;
	}
	
	public String analyzing(){
		String result = "";
		String temp[] = inputStreamString.split("=");
		if("chat".equals(temp[0])){
			result = temp[1];
		}
		return result;
	}
}
