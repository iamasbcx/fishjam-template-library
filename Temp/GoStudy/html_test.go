/*******************************************************************************
html -- 实现了一个HTML5兼容的分词器和解析器

html/template -- 自动构建HTML输出，并可防止代码注入
  可以将HTML从业务逻辑程序中抽离出来形成独立的模版文件，业务逻辑程序
  负责处理业务逻辑部分和提供模版需要的数据，模版文件负责数据要表现的具体形式。然后模版解析器将
  这些数据以定义好的模版规则结合模版文件进行渲染，最终将渲染后的结果一并输出，构成一个完整的网页
  模版文件中以双大括号区分模版代码和HTML代码: {{ 模版代码 }}

*******************************************************************************/
package gostudy

import (
	//"encoding/gob"
	//"fmt"
	"testing"
)

func TestHtmlTemplate(t *testing.T) {

}
