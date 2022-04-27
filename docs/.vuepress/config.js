const { description } = require('../../package')

module.exports = {
  /**
   * Ref：https://v1.vuepress.vuejs.org/config/#title
   */
  title: 'RoundRadio RF IoT Project Documentation',
  /**
   * Ref：https://v1.vuepress.vuejs.org/config/#description
   */
  description: description,

  base: '/roundradio-rf-iot/',

  /**
   * Theme configuration, here is the default theme configuration for VuePress.
   *
   * ref：https://v1.vuepress.vuejs.org/theme/default-theme-config.html
   */

  themeConfig: {
    sidebarDepth: 5,
    sidebar: [
      '/',
      '/hw-design/',
      '/sw-design/'
    ]
  },
  markdown: {
		lineNumbers: true,
		extendMarkdown: md => {
		  md.set({ html: true })
		  md.use(require('markdown-it-plantuml'))
		}
	}
}
